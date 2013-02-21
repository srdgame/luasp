
<? dotmpl "header.lsp" ?>

<table width="100%">
<?
for k,v in pairs(conf) do
?>
	<tr>
		<td align="right">
			<?=k?>:
		</td>
		<td align="left">
			<?=v?>
		</td>
	</tr>
<?
end
?>
</table>

<? dotmpl "footer.lsp" ?>
