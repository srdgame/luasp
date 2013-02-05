require('luaredis')

local client = redis.connect(params)
client:select(15) -- for testing purposes

client:set('foo', 'bar')
local value = client:get('foo')
print('redis value '..value)
