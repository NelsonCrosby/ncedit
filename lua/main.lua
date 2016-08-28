
local Screen = require('ncedit.screen')
-- local EchoPanel = require('ncedit.panels.echo')

-- Lifecycle handler function
-- Responsible for controlling ncedit
function ncedit(pname, ...)
    local file = ...
    -- Trivial hello-world implementation
    local screen = new(Screen)
    screen:hello('Hi, World!')
    screen:close()  -- Not needed (done by gc), but good practice
end

-- Return program lifecycle function
return ncedit
