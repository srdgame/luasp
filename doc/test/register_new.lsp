<? dotmpl "header.lsp" ?>

<?
email = nil
username = nil
passwd = nil
passwd2 = nil

if args_post then
	email = args_post.email
	username = args_post.username
	passwd = args_post.passwd
	passwd2 = args_post.passwd2
end
?>

<? 
function valid_reg(username, passwd, passwd2, email)
	if not username then
		return false, "User name empty"
	end
	if passwd ~= passwd2 then
		return false, "Password typed incorrect!"
	end
	if not email then
		return false, "User email  empty"
	end
	return true
end
?>

<?
	local r, err = valid_reg(username, passwd, passwd2, email)
?>
<?
	if r then
?>
<? 
	local client = redis.connect(params)
	client:select(15) -- for testing purposes
	local pwd = client:set(username..'_pw', passwd)
?>
<p>
注册成功
<p><a href="." >回到主页</a></p>
</p>
<?else?>
<p>
注册失败 <? echo(err) ?>
<?
	for k,v in pairs(args_post) do
		print(k..' - '..v) print("<br>")
	end
?>
</p>
<?end?>

<? dotmpl "footer.lsp" ?>
