

local Async = new.class()

function Async:init()
    self.loop = {}
end

function Async:begin()
    while #(self.loop) > 0 do
        local i = 1
        while i <= #(self.loop) do
            local co = self.loop[i]
            local status, msg = coroutine.resume(co)
            if not status then
                error(msg)  -- TODO: Some better form of error handling
            end

            if coroutine.status(co) == 'dead' then
                table.remove(self.loop, i)
            else
                i = i + 1
            end
        end
    end
end

function Async:addfunc(f)
    if type(f) == 'function' then
        f = coroutine.create(f)
    end

    if type(f) == 'thread' then
        table.insert(self.loop, f)
    else
        error('cannot add ' .. type(f) .. ' ' .. f .. ' to event loop')
    end
end

Async.__call = Async.addfunc

return new(Async)
