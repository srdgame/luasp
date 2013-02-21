<?#comment?>

<? dotmpl "header.lsp" ?>

	<style>
	label {
		display: inline-block; width: 5em;
	}
	fieldset div {
		margin-bottom: 2em;
	}
	fieldset .help {
		display: inline-block;
	}
	.ui-tooltip {
		width: 210px;
	}
	</style>
	<script>
	$(function() {
		var tooltips = $( "[title]" ).tooltip();
	});
	</script>

<form action="register_new.lsp" method="post">
	<fieldset>
		<h2>注册</h2>
		<p>
		<label for="username">用户名:</label>
		<input type="text" id="username" name="username" title="Please provide your username." size="5">
		</p>
		<p>
		<label for="passwd">密码:</label>
		<input type="password" id="passwd" name="passwd" title="Please provide your password." size="5">
		</p>
		</p>
		<label for="passwd2">重复密码:</label>
		<input type="password" id="passwd2" name="passwd2" title="Your home or work address." size="5">
		</p>
		<p>
		<label for="email">邮箱:</label>
		<input type="text" id="email" name="email" title="Your email address." size="5">
		</p>
		<p>
		<button name="submit" type="submit" id="submit">注册</button>
		</p>
	</fieldset>
</form>

<? dotmpl "footer.lsp" ?>

