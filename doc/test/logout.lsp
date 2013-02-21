
<? dotmpl "header.lsp" ?>

<? 
local user = auth.user
if auth.valid and auth.user then
	auth.logout(auth.user)
end
?>

<?=auth.valid?>
<?=auth.user?>
<? if user then?>
<p> 用户:<?=user?> 已经登出 </p>
<?end?>

<p><a href="." >回到主页</a></p>
