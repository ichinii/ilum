target = NewTarget("data")

function target:configure(settings)
  local datasrc_dir = "datasrc"
  local datadst_dir = "data"

  local data_files = CollectRecursive(PathJoin(datasrc_dir, "*"))

  for i, file in pairs(data_files) do
    local target = file:gsub("^" .. datasrc_dir .. "/", "")
    target = PathJoin(PathJoin(build_dir, datadst_dir), target)
    AddJob(target, "copy " .. file .. " > " .. target , "cp " .. file .. " " .. target)
    AddDependency(target, file)
    AddDependency(self.name, target)
  end
end
