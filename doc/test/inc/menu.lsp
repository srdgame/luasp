<style>
	.ui-menu { width: 100px; }
</style>
<script>
	$(function() {
		$( "#menu_login" ).menu();
		$( "#menu_about" ).menu();
		$( "#menu_contact" ).menu();
		$( "#menu_more" ).menu();
	});
</script>
<table  bgcolor="white" cellpadding="4" cellspacing="4" border="0">
	<tr bgcolor="white">
		<td>
			<ul id="menu_login">
				<li><a href="login.lsp">登录</a></li>
			</ul>
		</td>
		<td>
			<ul id="menu_about">
				<li><a href="about.lsp">关于</a></li>
			</ul>
		</td>
		<td>
			<ul id="menu_contact">
				<li><a href="contact.lsp">联系</a></li>
			</ul>
		</td>
		<td>
			<ul id="menu_more">
				<li>
				<a>More</a>
				<ul>
					<li class="ui-state-disabled"><a href="#">更多</a></li>
					<li><a href="#">读我</a></li>
					<li><a href="#">blabla</a></li>
				</ul>
				</li>
			</ul>
		</td>
	</tr>
</table>

