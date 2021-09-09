Import("env")

# access to global construction environment
print(env)

env.Execute("cd web && npm run build")
