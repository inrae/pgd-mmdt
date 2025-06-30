<?php

# Nom du fichier: home.inc
# Auteur(s): D.JACOB 
# Copyright: (c) INRAE - 2015-2025
#

?>

<!DOCTYPE html>
<html lang="fr">
<head>
	<title>Maggot - Home</title>
	<?= $HEAD ?>
	<link rel="stylesheet" href="<?= $PATH ?>css/home.css">
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
<div style="color: #557ca7; font-size: 20px;"><?= $menu1; ?></div>
<br>

<section>

<h1><img src="./img/logo.svg?raw=true" alt="" title="Maggot" style="width:100px;"> &nbsp;&nbsp;&nbsp;&nbsp;<span id="title"><?= $MAINTITLE; ?></span></h1>
<br>

<div style="background-image: url(img/banner-background.png); background-repeat: no-repeat; background-size: cover; padding: 20px 40px;">
<center><font style="font-size:32px; color:#e3e4ed; font-weight: 700;"><?= $BANNER; ?></font></center>
</div>
<br>

<center>

<!-- Left panel -->
<div class="box-home box-home-first"><div style="padding: 35px;">
<font style="font-size:18px; color: #3b618a;"><?= $HTMLTEXT; ?></font>
</div></div>

<!-- Right box -->
<div class="box-home"><div style="padding: 20px;">
<div class="container">
<?php foreach ($OPTGROUPS as $row => $features): ?>
	<?php if (!empty($features)): ?>
		<div class="row mb-2 justify-content-center">
		<?php $colWidth = intdiv(12, count($features)); ?>
		<?php foreach ($features as $opt): ?>
			<div class="col-12 col-md-<?= $colWidth ?> mb-0">
				<div class="card-radius card feature-box text-center shadow" onclick="location.href='<?= $opt['url'] ?>'">
				<div class="card-body"><div class="mb-0"><center>
					<table><tr><td><i class="bi bi-<?= $opt['icon'] ?>" style="font-size: 2rem;"></i></td>
					<td>&nbsp;</td>
					<td><span class="card-title" data-bs-toggle="tooltip" data-bs-offset="150,-20" 
							data-bs-placement="top" data-bs-html="true"
							title="<?= htmlspecialchars($opt['info']) ?>"><?= htmlspecialchars($opt['label']) ?></span></td></table>
				</center></div></div>
				</div>
			</div>
		<?php endforeach; ?>
		<?php if (in_array($opt['row'], $ROW_SEPARATOR) ): ?><div><hr></div><?php endif; ?>
		</div>
	<?php endif; ?>
<?php endforeach; ?>
</div>
</div></div>

</center>
</section>

<br/><br/><br/><br/>
