<?php

# Nom du fichier: about.inc
# Auteur(s): D.JACOB 
# Copyright: (c) INRAE - 2015-2025
#

// Badges
$parser = new ParsedownExtra();
$badges = preg_replace('/<a(.*?)href="([^"]+)"(.*?)>/i','<a$1href="$2"$3 target="_blank">', $parser->text('
[![SciCrunch](https://img.shields.io/badge/RRID-SCR_025261-green)](https://scicrunch.org/resolver/RRID:SCR_025261)
[![bio.tools](https://img.shields.io/badge/bio.tools-maggot-orange)](https://bio.tools/maggot)
[![RDA:MSC](https://img.shields.io/badge/RDA:MSC-t81-brown)](https://rdamsc.bath.ac.uk/msc/t81)
[![CatOPIDoR](https://img.shields.io/badge/CatOPIDoR-maggot-magenta)](https://cat.opidor.fr/index.php/Maggot)
[![HAL](https://img.shields.io/badge/HAL-04589638-lightseagreen)](https://hal.inrae.fr/hal-04589638)
'));

?>

<!DOCTYPE html>
<html lang="fr">
<head>
	<title>Maggot - Home</title>
	<?= $HEAD ?>
	<link rel="stylesheet" href="<?= $PATH ?>css/home.css">
	<style>h2 {font-size:20px; font-weight:normal; text-align:left; color:#25318d}</style>
	<script>
		var maxWidth=1200;
		$(document).ready(function() {
			$(window).on('resize', function(){ 
				$('#colitems').prop("rowspan", $( window ).width()>maxWidth ? 3 : -1 );
				$('#title').css('font-size', $( window ).width()>maxWidth ? '32px' : '28px' );
			});
			var tooltipTriggerList = [].slice.call(document.querySelectorAll('[data-bs-toggle="tooltip"]'))
			tooltipTriggerList.forEach(function (tooltipTriggerEl) {
				new bootstrap.Tooltip(tooltipTriggerEl)
			});
		});
	</script>
</head>
<body>

<body style="zoom: <?= $ZOOMWP ?>">
<?= $infolog; ?>
<div style="color: #557ca7; font-size: 20px;"><?= $menu2; ?></div>
<br>

<section>

<h1><img src="../img/logo.svg?raw=true" alt="" title="Maggot" style="width:100px;"> &nbsp;&nbsp;&nbsp;&nbsp;<span id="title"><?= $MAINTITLE; ?></span></h1>
<br>

<div style="background-image: url('../img/banner-background.png'); background-repeat: no-repeat; background-size: cover; padding: 20px 40px;">
<center><font style="font-size:32px; color:#e3e4ed; font-weight: 700;">Maggot, a research data cataloging tool - (v. <?= $VERSION ?>)</font></center>
</div>
<br>
<center>

<!-- Left panel -->
<div class="box-home" style="min-width:250px; min-height: 400px;"><div style="padding: 15px;">
<img src="<?= $PATH ?>img/maggot_overview.png?raw=true" width="450" height="350" alt="Overview"/>
</div></div>

<!-- Right box -->
<div class="box-home" style="width:800px; min-height: 400px;"><div style="padding: 20px;">
<?= $badges ?>

<h2>Links</h2>
<ul>
<li><b>Documentation</b> : <a href="https://inrae.github.io/pgd-mmdt/" target="_blank">https://inrae.github.io/pgd-mmdt/</a></li>
<li><b>Source code on Github</b> : <a href="https://github.com/inrae/pgd-mmdt" target="_blank">https://github.com/inrae/pgd-mmdt</a></li>
<li><b>Issues tracker</b> : <a href="https://github.com/inrae/pgd-mmdt/issues" target="_blank">https://github.com/inrae/pgd-mmdt/issues</a></li>
</ul>

<br>

<h2>Change Log</h2>
<ul>
<li><b>Change log on Github</b> : <a href="https://github.com/inrae/pgd-mmdt/blob/main/CHANGES.md" target="_blank">https://github.com/inrae/pgd-mmdt/blob/main/CHANGES.md</a></li>
</ul>

<br>

<h2>License</h2>
<ul><li>GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007 - <a href="https://www.gnu.org/licenses/gpl-3.0.html" target="_blank">GPLv3</a></li></ul>

</div></div>

</center>
</section>

<br/><br/><br/><br/>
