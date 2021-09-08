Import("env")

# access to global construction environment
print(env)

env.Execute("npm run build")
