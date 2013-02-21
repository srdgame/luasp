<?# This is comment ?>

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
?>

<?
local params = {
    host = '127.0.0.1',
    port = 6379,
}

?>

<?
local is_user_valid = true 
function check_user_valid (user, pass) 
	if not user or not pass then
		return false
	end
	local client = redis.connect(params)
	client:select(15) -- for testing purposes
	local pwd = client:get(user..'_pw')

	if pwd == pass then
		return true
	else
		return false
	end
end

is_user_valid = check_user_valid(user, password)

?>

<html lang="zh">
	<head>
		<meta charset="utf-8">
		<title>Hello from luasp</title>
		<link href="css/smoothness/jquery-ui-1.10.0.custom.css" rel="stylesheet">
		<script src="js/jquery-1.9.0.js"></script>
		<script src="js/jquery-ui-1.10.0.custom.js"></script>
		<script>
			$(function() {
				$( "#msgdlg" ).dialog({
					autoOpen: false,
					modal: true,
					show: {
						effect: "blind",
						duration: 200
					},
					hide: {
						effect: "explode",
						duration: 200
					}
				});

				$( "#button" ).button({
					icons: {
						primary: "ui-icon-check"
					},
					text: true
				})
				.click(function() {
					<?if is_user_valid then ?>
					$( "#msgdlg" ).dialog( "open" );
					<?end?>
				});
			});
		</script>
		<? dotmpl "style.lsp" ?>
	</head>
<body>
	<?if not is_user_valid then ?>
		<form action="login.lsp" method="post">
			<table border = "0">
				<tr>
					<td>用户:</td>
					<td align="center"><input type="text" name="user" size="5" maxlength="12"></td>
				</tr>
				<tr>
					<td>密码:</td>
					<td align="center"><input type="password" name="password" size="5" maxlength="12"></td>
				</tr>
				<tr>
					<td>
					</td>
					<td>
						<button id="button">Login</button>
					</td>
				</tr>
			</table>
		</form>
	<?else?>
		<button id="button">Show</button>
	<?end?>

	<div id="msgdlg" title="消息">
		User: <?=user?> <br>
		Password: <?=password?> <br>
	</div>

</body>

</html>
