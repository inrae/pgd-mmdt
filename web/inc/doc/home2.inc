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
  <?php echo $HEAD ?>
  <style>
	.btn-primary { background-color: #fff; border-color: #fff; }
	.box-home { min-width: 642px; min-height: 450px; min-height: 400px; }
	@media (min-width: 620px) { .container, .container-lg, .container-md, .container-sm, .container-xl, .container-xxl { max-width: 600px; } }
	.feature-box { min-height: unset; cursor: pointer; transition: transform 0.2s; }
	.feature-box:hover { transform: scale(1.03); }
	.card-body { padding: 0.35rem 0.65rem; }
	.card-radius { border-radius: 0.45rem !important; }
	.row { --bs-gutter-x: 0.6rem; }
    .card-title { font-size: 1.2rem; color: #25318d; }
	.shadow { box-shadow: 0 0.2rem 0.2rem rgba(0, 0, 0, .15) !important; }
	.tooltip-inner {
		background-color: #343a40; color: #ffffff;
		font-size: 1.2rem; font-family: 'Segoe UI', sans-serif;
		max-width: 250px; padding: 0.5rem 0.75rem; border-radius: 0.5rem;
		text-align: left; box-shadow: 0px 4px 8px rgba(0, 0, 0, 0.2);
	}
	.tooltip.bs-tooltip-top .tooltip-arrow::before { border-top-color: #343a40; }
	.feature-box:hover {border-color: #337ab7; transform: scale(1.05); }
  </style>
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
<?php echo $infolog; ?>
<div style="color: #557ca7; font-size: 20px;"><?= $menu1; ?></div>
<br>

<section>

<h1><img src="./docs/logo.svg?raw=true" alt="" title="Maggot" style="width:100px;"> &nbsp;&nbsp;&nbsp;&nbsp;<span id="title"><?= $MAINTITLE; ?></span></h1>
<br>

<div style="background-image: url(docs/banner4-background.png); background-repeat: no-repeat; background-size: cover; padding: 20px 40px;"><center>
<font style="font-size:32px; color:#e3e4ed; font-weight: 700;"><?= $BANNER; ?></font>
</center>
</div>
<br><br>

<center>

<!-- Left panel -->
<div class="box-home box-home-first"><div style="padding: 35px;">
<font style="font-size:18px; color: #3b618a;"><?= $HTMLTEXT; ?></font>
</div></div>

<!-- Right box -->
<div class="box-home"><div style="padding: 20px;">

<div class="container">
<?php foreach ($grouped as $row => $features): ?>
	<?php if (!empty($features)): ?>
		<div class="row mb-2 justify-content-center">
		<?php
			$count = count($features);
			$colWidth = intdiv(12, $count);
		?>
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
		<?php if (in_array($opt['row'], $row_separator) ): ?>
			<div><hr></div>
		<?php endif; ?>
		</div>
	<?php endif; ?>
<?php endforeach; ?>
</div>

</div></div>

</center>
</section>

<br/><br/><br/><br/>
