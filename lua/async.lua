

local Async = new.class()

function Async:init()
    self.q = {}
end

-- Run event loop
function Async:begin()
    while #(self.q) > 0 do
        -- Loop through queue
        -- Loop must be resistant to mutation
        local i = 1
        while i <= #(self.q) do
            -- Resume next coroutine
            local co = self.q[i]
            local status, msg = coroutine.resume(co)
            -- Handle possible error
            if not status then
                error(msg)  -- TODO: Some better form of error handling
            end

            if coroutine.status(co) == 'dead' then
                table.remove(self.q, i)
                -- Current i is now next coro in queue
            else
                -- Advance to next coro in queue
                i = i + 1
            end
        end
        -- Finished this round through the queue
    end
    -- Queue is emptied
end

-- Add a coroutine to event queue
-- If a function is passed a coroutine
--  will be created from it.
function Async:addfunc(f)
    if type(f) == 'function' then
        f = coroutine.create(f)
    end

    if type(f) == 'thread' then
        table.insert(self.q, f)
    else
        error('cannot add ' .. type(f) .. ' ' .. f .. ' to event loop')
    end
end

-- For simplicity, instances can be called
--  directly to add to the queue
Async.__call = Async.addfunc

-- This module's export will be an instance,
--  ready to use as local async = require('async')
return new(Async)
