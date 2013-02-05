<?# This is comment ?>

<html>
<head><title>Hello from luasp</title></head>

<body>

Session: <?=env.session?><br>

<?print('Your IP address: '..env.remote_ip)?><br>

URI: <?=env.uri?><br>

<?
	local con, err = mysql.open('root:19840310@localhost', 'test')
	if not con then
		print('open failed'..err)
	else
		local sql = con:escape('SELECT * FROM test.logs;')
		print('sql:'..sql..'<br>')
		local rs = con:query(sql)
		--print('row count '..rs:fields_count())
		print('row count '..con:affected_rows()..'<br>')
	end
?>

<?
local params = {
    host = '127.0.0.1',
    port = 6379,
}

local client = redis.connect(params)
client:select(15) -- for testing purposes

client:set('foo', 'bar')
local value = client:get('foo')
print('redis value '..value)
?>

</body>

</html>
