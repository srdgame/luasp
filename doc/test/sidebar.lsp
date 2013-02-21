<div id="sidebar">
	<div id="user_links">
		<ul>
			<?if auth.valid then?>
			<li><a class="logout_link" href="logout.lsp">Log Out</a></li>
			<?else?>
			<li><a class="login_link" href="login.lsp">Log In</a></li>
			<?end?>
		</ul>
	</div>
	<div id="about_info">
		<p><img src="images/profile.jpg" alt="about_info" />
			<ul>
				<li><a class="contact_link" href="contact.lsp">Contact</a></li>
			</ul>
		</p>
	</div>
	<div id="asides">
		<div id="elsewhere">
			<h5>Elsewhere</h5>
			<ul id="social_sites">
				<li><a class="twitter_link" href="http://twitter.com/dirkchang">@DirkChang on Twitter</a></li>
			</ul>
		</div>
		<div id="user_controls">
			<?if auth.valid then?>
			<h5>Welcome!</h5>
			<ul>
				<li><a class="user_link" href="userinfo.lsp"><?=auth.user?></a></li>
			</ul>
			<?else?>
			<h5>Welcome, Guest!</h5>
			<ul>
				<li><a class="lost_password_link" href=".?action=lost_password">Lost Password?</a></li>
			</ul>
			<?end?>
		</div>

		<div id="page_list">
			<h5>Pages</h5>
			<ul>
				<li>
				<a href="about.lsp">About</a>
				</li>
			</ul>
		</div>

		<div id="archive_list">
			<h5>Archives</h5>
			<ul>

				<li>
				<a href=".?action=archive&amp;year=2013&amp;month=02">February 2013</a>
				<span class="sub">(1)</span>
				</li>

				<li><a href=".?action=archive">All &rarr;</a></li>
			</ul>
		</div>

	</div>
</div>
