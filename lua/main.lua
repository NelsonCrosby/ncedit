local argv = ...


local f = io.open(argv[1], 'r')

local lines = {}
for l in f:lines() do
    table.insert(lines, l)
end


for i, v in ipairs(lines) do
    print(i, v)
end




f:close()
