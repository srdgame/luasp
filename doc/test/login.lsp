<?# This is comment ?>

<? dotmpl "header.lsp" ?>

<? if not auth.valid then ?>

<?
local user = nil
local password = nil
if env.method == 'POST' then
	if args_post then
		user = args_post.user
		password = args_post.password
	end
else
	if args then
		user = args.user
		password = args.password
	end
end

auth.valid, auth.err = auth.check_user_valid(user, password)

?>

<?if not auth.valid then ?>
<h2>登录</h2>
<form action="login.lsp" method="post">
	<p>
	<label for="user">用户:</label>
	<input type="text" name="user" size="5" maxlength="12">
	</p>
	<p>
	<label for="password">密码:</label>
	<input type="password" name="password" size="5" maxlength="12">
	</p>
	<p>
	<button name="submit" type="submit" id="submit">Login</button>
	</p>
	<p><a href="register.lsp">注册新用户</a></p>
</form>
<?
	if not auth.valid and auth.err then
		print(auth.err)
	end
?>

<?end?>
<?end?>

<? if auth.valid then ?>
<p> 用户:<?=auth.user?> 已经登录 </p>
<p><a href="." >回到主页</a></p>
<?end?>

<? dotmpl "footer.lsp" ?>

