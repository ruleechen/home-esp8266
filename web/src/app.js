((win) => {
  const _init = [];
  const vic = (fn) => {
    _init.push(fn);
  };
  vic._last = [];
  const run = () => {
    const fn = _init.shift() || vic._last.shift();
    if (typeof fn === "function") {
      Promise.resolve(fn())
        .then(run)
        .catch((ex) => console.error(ex));
    }
  };
  win.vic = vic;
  win.addEventListener("DOMContentLoaded", run);
})(window);

vic.query = (selector) => document.querySelector(selector);
vic.queryAll = (selector) => Array.from(document.querySelectorAll(selector));

vic.bytes = (bytes) =>
  bytes >= 1024 * 1024
    ? (bytes / 1024 / 1024).toFixed(2) + " MB"
    : bytes >= 1024
    ? (bytes / 1024).toFixed(2) + " KB"
    : `${bytes} BY`;

vic.ago = (millis, timestamp) => {
  const pad = (n) => `${n}`.padStart(2, "0");
  let span = (millis - timestamp) / 1000;
  let str = "";
  // d
  const oneDay = 86400;
  let days = span / oneDay;
  if (days > 1) {
    days = Math.floor(days);
    span -= days * oneDay;
    str += `${days}x24 `;
  }
  // h
  const oneHour = 3600;
  let hours = span / oneHour;
  if (hours > 1) {
    hours = Math.floor(hours);
    span -= hours * oneHour;
  } else {
    hours = 0;
  }
  str += pad(hours);
  // m
  const oneMinute = 60;
  let minutes = span / oneMinute;
  if (minutes > 1) {
    minutes = Math.floor(minutes);
    span -= minutes * oneMinute;
  } else {
    minutes = 0;
  }
  str += ":" + pad(minutes);
  // s
  const seconds = Math.floor(span);
  str += ":" + pad(seconds);
  // ret
  return str;
};

vic._navFns = [];
vic.appendNav = (fn) => vic._navFns.push(fn);
vic.getNav = () => {
  let items = [];
  if (vic._navFns) {
    vic._navFns.forEach((fn) => {
      const ret = fn(items);
      if (Array.isArray(ret)) {
        items = ret;
      }
    });
  }
  return m("p.nav", items);
};

vic._routeFns = [];
vic.appendRoute = (fn) => vic._routeFns.push(fn);
vic.getRoute = () => {
  let config = {};
  if (vic._routeFns) {
    vic._routeFns.forEach((fn) => {
      const ret = fn(config);
      if (ret) {
        config = ret;
      }
    });
  }
  return config;
};

vic.confirm = () => confirm("Are you sure you want to do it?");

vic.getLoading = () => [vic.getNav(), m("div.loading", "Loading")];

vic.navItem = (href, text) => {
  const curr = m.route.get();
  const match = curr === href || curr.indexOf(href + "/") === 0;
  return m(m.route.Link, { href, class: match ? "match" : "" }, text);
};

vic.mSelect = (name, value, options) => {
  return m(
    "select",
    { name },
    options.map((x) =>
      m("option", { value: x.value, selected: value === x.value }, x.text)
    )
  );
};

vic.mTable = (data) => {
  let rows = [];
  let cols = 0;
  if (data.header && data.header.length > 0) {
    cols = data.header.length;
    rows.push(
      m(
        "tr",
        data.header.map((c) => m("th", [c]))
      )
    );
  }
  if (data.rows && data.rows.length > 0) {
    if (cols === 0) {
      cols = data.rows[0].length;
    }
    rows = rows.concat(
      data.rows.map((row) =>
        m(
          "tr",
          row.map((cell) => m("td", [cell]))
        )
      )
    );
  }
  return m(`table.cols${cols}`, rows);
};

const mSelectionList = (type, name, values, list) => {
  const items = list.map((item) =>
    m("li", [
      m("input", {
        type,
        name,
        id: `rdo${name}${item.value}`,
        value: item.value,
        checked: values.includes(item.value),
      }),
      m("label", { for: `rdo${name}${item.value}` }, item.text),
    ])
  );
  return m("ul", items);
};

vic.mRadioList = (name, values, list) =>
  mSelectionList("radio", name, values, list);
vic.mCheckList = (name, values, list) =>
  mSelectionList("checkbox", name, values, list);

const HomeView = (() => {
  const state = {
    loading: true,
    data: {},
  };
  const oninit = () => {
    state.loading = true;
    m.request({
      method: "GET",
      url: "/status",
    }).then((res) => {
      state.loading = false;
      state.data = res;
      m.redraw();
    });
  };
  return {
    oninit,
    view() {
      if (state.loading) {
        return vic.getLoading();
      }
      const d = state.data;
      return [
        vic.getNav(),
        m("h3", "Home"),
        m("p", [
          vic.mTable({
            header: ["Status", ""],
            rows: [
              ["Boot", vic.ago(d.millis, 0)],
              ["Reset Reason", d.resetReason],
              ["Free Stack", vic.bytes(d.freeStack)],
              ["Free Heap", vic.bytes(d.freeHeap)],
              ["Max Free Block Size", vic.bytes(d.maxFreeBlockSize)],
              ["Heap Fragmentation", d.heapFragmentation],
            ],
          }),
          vic.mTable({
            header: ["Hardware", ""],
            rows: [
              ["Chip ID", d.chipId],
              ["CPU Freq", `${d.cupFreqMHz} MHz`],
              ["Flash ID", d.flashId],
              ["Flash Size", vic.bytes(d.flashSize)],
              ["Flash Size (Real)", vic.bytes(d.flashSizeReal)],
              ["Flash Speed", `${d.flashSpeedMHz} MHz`],
            ],
          }),
          vic.mTable({
            header: ["Software", ""],
            rows: [
              [
                "Sketch MD5",
                m("span.ellipsis", { title: d.sketchMD5 }, d.sketchMD5),
              ],
              ["Sketch Size", vic.bytes(d.sketchSize)],
              ["Sketch Free Space", vic.bytes(d.sketchFreeSpace)],
              ["SDK Version", d.sdkVersion],
              ["Core Version", d.coreVersion],
              ["Firmware Version", d.firmwareVersion],
            ],
          }),
        ]),
      ];
    },
  };
})();

const FileSystemView = (() => {
  const state = {
    loading: true,
    data: {},
  };
  const oninit = () => {
    state.loading = true;
    m.request({
      method: "GET",
      url: "/fs",
    }).then((res) => {
      state.loading = false;
      state.data = res;
      m.redraw();
    });
  };
  return {
    oninit,
    view() {
      if (state.loading) {
        return vic.getLoading();
      }
      const d = state.data;
      return [
        vic.getNav(),
        m("h3", "File System"),
        m("p", [m(m.route.Link, { href: "/fs/files" }, "Files")]),
        m("p", [
          vic.mTable({
            rows: [
              ["Total", vic.bytes(d.totalBytes)],
              ["Used", vic.bytes(d.usedBytes)],
              ["Max Path Length", d.maxPathLength],
              ["Max Open Files", d.maxOpenFiles],
              ["Block Size", vic.bytes(d.blockSize)],
              ["Page Size", vic.bytes(d.pageSize)],
            ],
          }),
        ]),
      ];
    },
  };
})();

const FileListView = (() => {
  const state = {
    loading: true,
    files: [],
  };
  const oninit = () => {
    state.loading = true;
    m.request({
      method: "GET",
      url: "/files",
    }).then((res) => {
      state.loading = false;
      state.files = res.files;
      m.redraw();
    });
  };
  return {
    oninit,
    view() {
      if (state.loading) {
        return vic.getLoading();
      }
      return [
        vic.getNav(),
        m("h3", "Files"),
        m("p", [m(m.route.Link, { href: "/fs" }, "< FS")]),
        m("p", [
          vic.mTable({
            rows: state.files.map(({ path, size }) => [
              m(
                m.route.Link,
                {
                  href: m.buildPathname("/fs/files/:path", { path }),
                },
                path
              ),
              vic.bytes(size),
            ]),
          }),
        ]),
      ];
    },
  };
})();

const FileItemView = (() => {
  const state = {
    loading: true,
    file: { name: "", size: 0, content: "" },
  };
  const request = (method, body) => {
    const path = m.route.param("path");
    return m.request({
      method,
      url: "/file",
      params: { path },
      body,
    });
  };
  const save = () => {
    request("POST", {
      content: vic.query("textarea").value,
    }).then((res) => {
      if (res.error) {
        alert(res.error);
      } else {
        oninit();
      }
    });
  };
  const remove = () => {
    if (vic.confirm()) {
      request("DELETE").then((res) => {
        if (res.error) {
          alert(res.error);
          return;
        }
        m.route.set("/fs/files");
      });
    }
  };
  const oninit = () => {
    state.loading = true;
    request("GET").then((res) => {
      state.file = res;
      state.loading = false;
      m.redraw();
    });
  };
  return {
    oninit,
    view() {
      if (state.loading) {
        return vic.getLoading();
      }
      return [
        vic.getNav(),
        m("h3", `${state.file.name} (${state.file.size} bytes)`),
        m("p", [m(m.route.Link, { href: "/fs/files" }, "< Files")]),
        m("div.form", [
          m("p", [m("textarea", { cols: 50, rows: 10 }, state.file.content)]),
          m("p", [
            m("button.btn", { onclick: save }, "Save"),
            m("button.btn.weak", { onclick: remove }, "Delete"),
          ]),
        ]),
      ];
    },
  };
})();

const WifiView = (() => {
  const state = {
    loading: true,
    data: {},
  };
  const oninit = () => {
    state.loading = true;
    m.request({
      method: "GET",
      url: "/wifi",
    }).then((res) => {
      state.loading = false;
      state.data = res;
      m.redraw();
    });
  };
  return {
    oninit,
    view() {
      if (state.loading) {
        return vic.getLoading();
      }
      const d = state.data;
      return [
        vic.getNav(),
        m("h3", "WiFi"),
        m("p", [m(m.route.Link, { href: "/wifi/list" }, "Join Wifi")]),
        m("p", [
          vic.mTable({
            rows: [
              [
                "Local Host",
                d.localHost
                  ? m("a", { href: `http://${d.localHost}.local` }, d.localHost)
                  : "",
              ],
              ["Mode", d.wifiMode],
              ["Joined", d.joined ? d.joined : "-"],
              [
                "STA IP",
                d.staAddress
                  ? m("a", { href: `http://${d.staAddress}` }, d.staAddress)
                  : "-",
              ],
              ["STA MAC", d.staMacAddress],
              [
                "AP IP",
                d.apAddress
                  ? m("a", { href: `http://${d.apAddress}` }, d.apAddress)
                  : "-",
              ],
              ["AP MAC", d.apMacAddress],
            ],
          }),
        ]),
      ];
    },
  };
})();

const WifiListView = (() => {
  const state = {
    loading: true,
    bssid: null,
    founds: [{ bssid: "", ssid: "", channel: 0, rssi: 10 }],
    password: "",
  };
  const scan = () => {
    state.password = vic.query("#txtPassword").value;
    oninit();
  };
  const join = () => {
    const bssidEl = vic.query("input[type=radio]:checked");
    if (!bssidEl) {
      alert("Please select wifi to join");
      return;
    }
    const passEl = vic.query("#txtPassword");
    const ap = state.founds.find((x) => x.bssid === bssidEl.value);
    m.request({
      method: "POST",
      url: "/wifi/join",
      body: Object.assign({}, { password: passEl.value }, ap),
    }).then((res) => {
      if (res.error) {
        alert(res.error);
      } else if (res.ip) {
        alert("Success! obtain ip: " + res.ip);
      }
      m.redraw();
    });
  };
  const oninit = () => {
    state.loading = true;
    m.request({
      method: "GET",
      url: "/wifi/list",
    }).then((res) => {
      state.loading = false;
      Object.assign(state, res);
      m.redraw();
    });
  };
  return {
    oninit,
    view() {
      if (state.loading) {
        return vic.getLoading();
      }
      return [
        vic.getNav(),
        m("h3", "Join WiFi"),
        m("div.form", [
          vic.mTable({
            rows: state.founds.map((x) => [
              m("input", {
                type: "radio",
                name: "bssid",
                id: x.bssid,
                value: x.bssid,
                checked: x.bssid === state.bssid,
              }),
              m("label", { for: x.bssid }, x.ssid),
              m("label", { for: x.bssid }, x.bssid),
              m("label", { for: x.bssid }, `${100 + x.rssi}%`), // the closer the value is to 0, the stronger the received signal has been.
            ]),
          }),
          m("p", [
            m("label", { for: "txtPassword" }, "Password"),
            m("input[type=text]", {
              id: "txtPassword",
              maxLength: 32,
              value: state.password,
            }),
          ]),
          m("p", [
            m("button.btn", { onclick: scan }, "Scan"),
            m("button.btn", { onclick: join }, "Join"),
          ]),
        ]),
      ];
    },
  };
})();

const OtaView = (() => {
  const state = {
    loading: true,
    data: {},
  };
  const fire = () => {
    const version = "";
    const otaType = "";
    m.request({
      method: "POST",
      url: "/ota/fire",
      body: { version, otaType },
    }).then((res) => {
      if (res.error) {
        alert(res.error);
      } else {
        m.redraw();
      }
    });
  };
  const oninit = () => {
    state.loading = true;
    m.request({
      method: "GET",
      url: "/ota",
    }).then((res) => {
      state.loading = false;
      state.data = res;
      m.redraw();
    });
  };
  return {
    oninit,
    view() {
      if (state.loading) {
        return vic.getLoading();
      }
      const d = state.data;
      return [
        vic.getNav(),
        m("h3", "OTA"),
        d.overTheWeb
          ? m("p", [
              m(m.route.Link, { href: "/ota/otw" }, "Update - Over The Web"),
            ])
          : null,
        m("div.form", [
          m("p", [
            vic.mTable({
              rows: [
                ["Flash Size", vic.bytes(d.flashSize)],
                ["Flash Size (Real)", vic.bytes(d.flashSizeReal)],
                ["Sketch Size", vic.bytes(d.sketchSize)],
                ["Sketch Free Space", vic.bytes(d.sketchFreeSpace)],
                ["Remote Latest", d.otaNewVersion],
                ["Local Firmware", d.otaVersion],
              ],
            }),
          ]),
          vic.mRadioList(
            "OtaType",
            ["sketch"],
            [
              { value: "all", text: "All" },
              { value: "sketch", text: "Sketch" },
              { value: "fs", text: "File System" },
            ]
          ),
          m("p", [
            m(
              "button.btn",
              { onclick: fire },
              "Load + Burn " + d.otaNewVersion
            ),
          ]),
        ]),
      ];
    },
  };
})();

const OtaOtwView = {
  view() {
    return [
      vic.getNav(),
      m("h3", "Over The Web"),
      m("p", [m(m.route.Link, { href: "/ota" }, "< OTA")]),
      m("div.form", [
        m("iframe", {
          width: "100%",
          height: "300",
          frameborder: "0",
          scrolling: "no",
          marginHeight: "0",
          marginWidth: "0",
          src: "/update",
        }),
      ]),
    ];
  },
};

const ResetView = (() => {
  const state = {
    loading: true,
  };
  const reset = () => {
    const values = vic
      .queryAll("input[name=Reset]:checked")
      .map((x) => x.value)
      .join(",");
    m.request({
      method: "POST",
      url: "/reset",
      body: { values },
    }).then((res) => {
      if (res.error) {
        alert(res.error);
      } else {
        m.redraw();
      }
    });
  };
  const oninit = () => {
    state.loading = false;
  };
  return {
    oninit,
    view() {
      if (state.loading) {
        return vic.getLoading();
      }
      return [
        vic.getNav(),
        m("h3", "Reset"),
        m("div.form", [
          vic.mCheckList(
            "Reset",
            [],
            [
              { value: "EspRestart", text: "ESP Restart" },
              { value: "EspReset", text: "ESP Reset" },
              { value: "EspEraseCfg", text: "ESP Erase Config" },
              { value: "WifiReset", text: "Reset WiFi" },
            ]
          ),
          m("p", [m("button.btn", { onclick: reset }, "Submit")]),
        ]),
      ];
    },
  };
})();

const NotfoundView = {
  view() {
    return "Oops... Notfound";
  },
};

vic.appendNav((items) =>
  items.concat([
    vic.navItem("/", "Home"),
    m("span", " | "),
    vic.navItem("/wifi", "WiFi"),
    m("span", " | "),
    vic.navItem("/fs", "FS"),
    m("span", " | "),
    vic.navItem("/ota", "OTA"),
    m("span", " | "),
    vic.navItem("/reset", "Reset"),
  ])
);

vic.appendRoute((config) =>
  Object.assign(config, {
    "/": HomeView,
    "/wifi": WifiView,
    "/wifi/list": WifiListView,
    "/fs": FileSystemView,
    "/fs/files": FileListView,
    "/fs/files/:path": FileItemView,
    "/ota": OtaView,
    "/ota/otw": OtaOtwView,
    "/reset": ResetView,
    "/404": NotfoundView,
  })
);

// start
vic(() => {
  const root = vic.query("div.main");
  m.route(root, "/", vic.getRoute());
});
