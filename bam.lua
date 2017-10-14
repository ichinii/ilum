
modules_dir = "modules"

-- include lua utils
do
  local utils = Collect(PathJoin(PathJoin(modules_dir, "util"), "*.lua"))
  for k, v in pairs(utils) do
    Import(v)
  end
end

-- variables
conf = ScriptArgs["conf"] or "debug"
build_dir = ScriptArgs["build_dir"] or "build"
build_dir = PathJoin(build_dir, conf)
src_dir = "src"

-- configure settings on creation
AddTool(function(settings)
	if conf == "debug" then
		settings.debug = 1
		settings.optimize = 0
	  settings.cc.flags:Add("-g")
	elseif conf == "release" then
		settings.debug = 0
		settings.optimize = 1
	end

	settings.cc.flags:Add("-Wall")
	settings.cc.flags:Add("-Wextra")
	settings.cc.flags_cxx:Add("--std=c++17")
	settings.cc.includes:Add("src")
	settings.cc.Output = function(settings, input)
		input = input:gsub("^src/", "")
		return PathJoin(PathJoin(build_dir, "obj"), PathBase(input))
	end
	settings.link.Output = function(settings, input)
		return PathJoin(build_dir, PathBase(input))
	end
end)



PseudoTarget("all")

-- load all targets
do
  local targets = {}
  do
    local target_files = Collect(PathJoin(PathJoin(modules_dir, "targets"), "*.lua"))
    for k, v in pairs(target_files) do
      Import(v)
      table.insert(targets, target)
    end
  end
  for k, v in pairs(targets) do
    v:configure(NewSettings())
    AddDependency("all", v.name)
  end
end

DefaultTarget("game")
