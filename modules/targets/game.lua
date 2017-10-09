target = NewTarget("game")

function target:configure(settings)
  settings.link.libs:Add("GL")
  settings.link.libs:Add("GLU")
  settings.link.libs:Add("GLEW")
  settings.link.libs:Add("glfw")

  local srcs = CollectRecursive(PathJoin("src", "*.cpp"))
  local objs = Compile(settings, srcs)
  local bin = Link(settings, "otic", objs)

  print(self.name)
  AddDependency(self.name, bin)
  AddDependency(self.name, "data")

  -- target "run"
  PseudoTarget("run")
  AddJob("run", "running game...", bin)
  AddDependency("run", self.name)
end
