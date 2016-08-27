
-- Not required, but entirely valid
local new = require('class')

test = new(Test, 1)
test:hello()


Test2 = new.class(Test)

function Test2:goodbye()
    self:hello()
    print('Goodbye!')
end

test2 = new(Test2, 2)
test2:goodbye()
