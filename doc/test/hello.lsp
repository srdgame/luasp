<?# This is comment ?>

<html lang="zh">
	<head>
		<meta charset="utf-8">
		<title>Hello from luasp</title>
		<link href="css/smoothness/jquery-ui-1.10.0.custom.css" rel="stylesheet">
		<script src="js/jquery-1.9.0.js"></script>
		<script src="js/jquery-ui-1.10.0.custom.js"></script>
		<!--script src="development-bundle/ui/i18n/jquery.ui.datepicker-zh-CN.js"></script-->
		<script>
			$(function() {
				$( "#dialog" ).dialog({
					autoOpen: false,
					modal: true,
					/*
					show: {
						effect: "blind",
						duration: 1000
					},
					hide: {
						effect: "explode",
						duration: 1000
					}
					*/
				});

				$.datepicker.setDefaults( $.datepicker.regional[ "zh-CN" ] );
				$( "#datepicker_main" ).datepicker( {
					inline: true,
					dateFormat: "yy/mm/dd",
					onSelect : function(selectedDate) {
						/*window.location.href="login.lsp?backurl="+window.location.href; */
						//window.navigate("hello.lsp");
						/*
						alert("返回");
						window.history.back(-1);
						*/
						var url = "show.lsp?";
						url += selectedDate;
						self.location=url;
						//top.location='xx.lsp';
					}
				});
				$( "#button" ).button({
					icons: {
						primary: "ui-icon-help"
					},
					text: false
				})
				.click(function() {
					$( "#dialog" ).dialog( "open" );
				});
			});
		</script>
		<style>
			body {
				font: 62.5% "Trebuchet MS", sans-serif;
				margin: 50px;
			}
		</style>
	</head>

<body>

	<button id="button">Button with icon only</button>
	<button>Button with icon only</button>

	<div id="dialog" title="Basic dialog">
		<h3>Info:</h3>
		<?# comment ?>
		<?
			print('luasp version : '..version("luaredis"))
			print('<br>')
			print('UUID: '..uuid_gen())
			print('<br>')
		?>
		<h3>Modules:</h3>
		<? 
			local t = module_list() 
			for k,v in pairs(t) do
				print(k..':'..v..'<br>')
			end
		?>
	</div>

	<?print('Your IP address: '..env.remote_ip)?><br>

	<h1>
		URI: <?=env.uri?><br>
	</h1>

	<div id="datepicker_main"></div>

</body>

</html>
