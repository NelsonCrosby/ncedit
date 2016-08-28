

function ncedit(pname, ...)
    local argv = {...}
    print(0, pname)
    for k, v in pairs(argv) do
        print(k, v)
    end
end

-- Return program lifecycle function
return ncedit
