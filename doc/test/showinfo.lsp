<?
function showinfo()
	print('luasp version : '..version("luaredis"))
	print('<br>')
	print('UUID: '..uuid_gen())
	print('<br>')
end
?>

<?
function lspinfo()
?>
	<h3>Info:</h3>
<?
	showinfo()
?>
	<h3>Modules:</h3>
<?
	local t = module_list() 
	for k,v in pairs(t) do
		print(k..':'..v..'<br>')
	end
end
?>
