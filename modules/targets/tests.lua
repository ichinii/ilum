target = NewTarget("tests")

function target:configure(settings)
  local tests = Collect(PathJoin("tests", "*.lua"))
  for k, v in pairs(tests) do
    local settings = TableDeepCopy(settings)

    Import(v)
    target:configure(TableDeepCopy(settings))
    AddDependency(self.name, target.name)
  end
end
