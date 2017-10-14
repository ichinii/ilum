target = NewTarget("game")

function target:configure(settings)
  settings.link.libs:Add("GL")
  settings.link.libs:Add("GLU")
  settings.link.libs:Add("GLEW")
  settings.link.libs:Add("glfw")

  local srcs = CollectRecursive(PathJoin("src", "*.cpp"))
  local objs = Compile(settings, srcs)
  local bin = Link(settings, "ilum", objs)

  print(self.name)
  AddDependency(self.name, bin)
  AddDependency(self.name, "data")

  local filename = PathFilename(bin)

  -- target "run"
  PseudoTarget("run")
  AddJob("run", "running "..filename.."...", "(cd "..build_dir.." && ./"..filename..")")
  AddDependency("run", self.name)

  -- target "gdbgui"
  PseudoTarget("gdbgui")
  AddJob("gdbgui", "running "..filename.." in gdbgui...", "(cd "..build_dir.." && gdbgui "..filename..")")
  AddDependency("gdbgui", self.name)
end
