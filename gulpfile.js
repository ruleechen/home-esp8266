const del = require("del");
const gulp = require("gulp");
const gzip = require("gulp-gzip");
const uglify = require("gulp-uglify");
const cleanCSS = require("gulp-clean-css");
const rename = require("gulp-rename");

const resource = {
  css: ["web/*.css"],
  js: ["web/*.js", "!web/*.min.js"],
  copy: ["web/*.htm"],
  copy_gzip: ["web/*.min.js"],
};

const paths = {
  dist: "data/web",
};

function clean() {
  return del([paths.dist], { force: true });
}

function copy() {
  return gulp.src(resource.copy).pipe(gulp.dest(paths.dist));
}

function copy_gzip() {
  return gulp.src(resource.copy_gzip).pipe(gzip()).pipe(gulp.dest(paths.dist));
}

function styles() {
  return gulp
    .src(resource.css)
    .pipe(cleanCSS())
    .pipe(
      rename({
        suffix: ".min",
      })
    )
    .pipe(gzip())
    .pipe(gulp.dest(paths.dist));
}

function scripts() {
  return gulp
    .src(resource.js)
    .pipe(uglify())
    .pipe(
      rename({
        suffix: ".min",
      })
    )
    .pipe(gzip())
    .pipe(gulp.dest(paths.dist));
}

function watch() {
  gulp.watch(resource.css, styles);
  gulp.watch(resource.js, scripts);
}

const build = gulp.series(
  clean,
  gulp.parallel(copy, copy_gzip, styles, scripts)
);

// export gulp tasks
exports.clean = clean;
exports.styles = styles;
exports.scripts = scripts;
exports.watch = watch;
exports.build = build;
exports.default = build;
