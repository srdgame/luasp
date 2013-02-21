<?# this is the header file for this system?>

<? dotmpl "inc/conf.lsp" ?>
<? dotmpl "inc/auth.lsp" ?>
<!DOCTYPE html>
<html xmlns="http://www.w3.org/1999/xhtml">
	<head profile="http://gmpg.org/xfn/11">
		<meta http-equiv="Content-type" content="text/html; charset=utf-8" />
		<title><?=conf.title?></title>
		<meta name="description" content="" />
		<meta name="generator" content="luasp" />
		<link href="css/smoothness/jquery-ui-1.10.0.custom.css" rel="stylesheet">
		<link href="css/screen.css" rel="stylesheet">
		<script src="js/jquery-1.9.0.js"></script>
		<script src="js/jquery-ui-1.10.0.custom.js"></script>
		<script src="theme.js" type="text/javascript" charset="utf-8"></script>
		<? dotmpl("inc/style.lsp") ?>
		<? dotmpl("inc/script.lsp") ?>
	</head>
	<body>
		<div id="container">
			<div id="header">
				<div id="search_box">
					<form action="." method="get" accept-charset="utf-8">
						<fieldset>
							<input type="hidden" name="action" value="search" id="action" />
							<input type="text" id="search" name="query" value="" x-webkit-speech />
						</fieldset>
					</form>
				</div>
				<div class="header_box">
					<h1><a href="."><?=conf.title?></a>
					</h1>
				</div>
			</div>
			<div id="wrapper">
				<? dotmpl "sidebar.lsp"?>
				<div id="content">
