<?php
	$links[0]="https://www.ouvrirlascience.fr/dmp-opidor-outil-pour-faciliter-la-redaction-de-plans-de-gestion-de-donnees/";
	$links[1]="https://www.ouvrirlascience.fr/deuxieme-plan-national-pour-la-science-ouverte/";
	$links[2]="https://entrepot.recherche.data.gouv.fr/";
	$links[3]="https://www.ouvrirlascience.fr/fair-principles/";
	$links[4]="https://www.u-picardie.fr/open-science/wp-content/uploads/2021/07/Fiche-Synthetique-Metadonnees.pdf";
	$links[5]="./doc/meta";
	$links[6]="https://inrae.github.io/pgd-mmdt/tutorial/";
?>

<!DOCTYPE html>
<html lang="en" >

<head>
	<title>Maggot - Home</title>
	<?php echo $HEAD ?>

	<style>.btn-primary { background-color: #fff; border-color: #fff; }
	.overlay_cl { top: 0px; padding: 20px; background-color: #fafafa; width: 500px; font-size: 20px; font-family: Arial; color: rgb(70, 130, 183); text-align: justify; display: none; position: absolute; border: solid 1px #555; box-shadow:  0 0 10px  rgba(0,0,0,0.6); -moz-box-shadow: 0 0 10px  rgba(0,0,0,0.6); -webkit-box-shadow: 0 0 10px  rgba(0,0,0,0.6); -o-box-shadow: 0 0 10px  rgba(0,0,0,0.6);}
	</style>

	<script>
		var maxWidth=1200;
		$(document).ready(function() {
			$('#image_map area').each(function() {
				var id = $(this).attr('id');
				$(this).mouseover(function(e) {
					mouseX = e.pageX;  mouseY = e.pageY;
					$('#overlay_' + id).css({'top':1.05*mouseY+75,'left':mouseX}).show();
				});
				$(this).mouseout(function() {
					var id = $(this).attr('id');
					$('#overlay_' + id).hide();
				});
			});
			$(window).on('resize', function(){ 
				$('#colitems').prop("rowspan", $( window ).width()>maxWidth ? 3 : -1 );
				$('#title').css('font-size', $( window ).width()>maxWidth ? '32px' : '28px' );
			})
		});
	</script>
</head>

<body style="zoom: <? echo $ZOOMWP ?>">

<div style="color: #557ca7; font-size: 20px;"><?php echo $menu1; ?></div>
<br>

<section>

<h1><img src="./docs/logo.svg?raw=true" alt="" title="Maggot" style="width:100px;"> &nbsp;&nbsp;&nbsp;&nbsp;<span id="title"><?php echo $MAINTITLE; ?></span></h1>
<br>

<div style="background-image: url(docs/Data-sharing-background.png); background-repeat: no-repeat; background-size: cover; padding: 20px 40px;"><center>
<font style="font-size:32px; color:white; font-weight: 700;">An ecosystem for producing and sharing metadata</font>
</center>
</div>
<br><br>

<center>

<!-- First box -->
<div class="box-home box-home-first"><div style="padding: 35px;">
<font style="font-size:18px; color: #3b618a;">
The <b>Maggot</b> tool allows users to describe the <a href="<? echo $links[4]; ?>" target="_blank">Metadata</a> of their datasets produced within a collective (research unit, platform, multi-partner project, etc.), thus making it possible
<ul>
	<li>to answer certain questions of the <b>Data Management Plan</b> (<a href="<? echo $links[0]; ?>" target="_blank">DMP</a>) concerning the <b>organization, documentation, storage and sharing of data</b> in the data storage space,</li>
	<li>to meet certain data and metadata requirements, referred to by the <a href="<? echo $links[1]; ?>" target="_blank">National Plan for Open Science</a> , in particular its second axis: “<b>Structure, Share and Open Research Data</b>” (<a href="<? echo $links[2]; ?>" target="_blank">Recherche Data Gouv</a>) in accordance with the <a href="<? echo $links[3]; ?>" target="_blank">FAIR principles</a>.</li>
</ul>
See <a href="<? echo $links[6]; ?>" target="_blank">Quick tutorial</a>.
</font>
</div></div>

<!-- Second box -->
<div class="box-home"><div style="padding: 20px;">
<p align="center">
<?php
	$overview_png = ($export_zenodo) ? $PRIVATE_ACCESS ? 'overview2c.png' : 'overview2b.png' : 'overview2.png';
	$coords = array( "7,8,464,43", "7,8,228,43", "230,8,464,43", "7,51,464,86", "7,94,219,161", "255,95,464,162", "471,226,712,299", "509,32,638,161", "206,221,364,382", "464,317,657,383" );
	if ($EXTERN==0) echo '<img src="docs/'.$overview_png.'?raw=true" width="722" height="400" alt="Overview" usemap="#image_map"/>';
	else echo '<img src="docs/overview3.png?raw=true" alt="Overview" usemap="#image_map"/>';
?>
	<br><i sty0le="color: #0b2a68;">Move the mouse over the image then click on the desired functionality</i>
	<map name="image_map" id="image_map">

<?php
	if ($EXTERN==0) {
		if ($PRIVATE_ACCESS) echo
	'<area href="./config/view"                 coords="'.$coords[1].'"  shape="rect" id="r1">
	 <area href="./access"                      coords="'.$coords[2].'"  shape="rect" id="r1b">';
		else echo
	'<area href="./config/view"                 coords="'.$coords[0].'"  shape="rect" id="r1">';
		echo
	'<area href="./dictionary"                  coords="'.$coords[3].'"  shape="rect" id="r2">
	 <area href="./entry"                       coords="'.$coords[4].'"  shape="rect" id="r3">
	 <area href="./search"                      coords="'.$coords[5].'"  shape="rect" id="r4">
	 <area href="./dataverse"                   coords="'.$coords[6].'"  shape="rect" id="r5">
	 <area href="'.$links[5].'" target="_blank" coords="'.$coords[7].'"  shape="rect" id="r6">';
	}
	else echo 
	'<area href="./dictionary"  coords="29,28,487,62"    shape="rect" id="r2">
	 <area href="./entry"       coords="30,72,489,138"   shape="rect" id="r3">
	 <area href="./dataverse"   coords="119,216,366,297" shape="rect" id="r5">';

	if ($FILEBROWSER==1)
		echo '<area href="'.$URL_FILEBROWSER.'" target="_blank" coords="'.$coords[8].'" shape="rect" id="r7">';
	if ($export_zenodo==0 && $FAIRLINK==1)
		echo '<area href="'.$URL_FAIR.'"        target="_blank" coords="'.$coords[9].'" shape="rect" id="r8">';
	if ($export_zenodo==1)
		echo '<area href="./zenodo"             target="_blank" coords="'.$coords[9].'" shape="rect" id="r8">';
?>
  </map>
</p>
</div></div>

</center>

<div id="overlay_r1" class="overlay_cl"><b>Configure</b> the terminology definition file</div>
<div id="overlay_r1b" class="overlay_cl"><b>Manage</b> private access keys</div>
<div id="overlay_r2" class="overlay_cl"><b>Edit</b> the dictionaries (need admin access to edit or just use 'maggot' user for viewing only)</div>
<div id="overlay_r3" class="overlay_cl"><b>Describe</b> a dataset using metadata of different types by facilitating their entry and avoiding their re-entry thanks to personalized dictionaries, then generate the metadata file</div>
<div id="overlay_r4" class="overlay_cl"><b>Search</b> datasets based on some metadata - assuming the metadata were previously deposited in the storage space</div>
<div id="overlay_r5" class="overlay_cl"><b>Publish</b> the metadata of datasets along with their data files into the <i>Recherche Data Gouv</i> repository</div>
<div id="overlay_r6" class="overlay_cl"><b>Metadata</b> documentation</div>
<div id="overlay_r7" class="overlay_cl"><b>File Browser</b> allows you to browse data directories using cloud-like software</div>
<?php
	if ($export_zenodo==0 && $FAIRLINK==1)
		echo '<div id="overlay_r8" class="overlay_cl"><b>FAIR principles</b> in action with Dataverse</div>';
	if ($export_zenodo==1)
		echo '<div id="overlay_r8" class="overlay_cl"><b>Publish</b> the metadata of datasets along with their data files into the <i>Zenodo</i> repository</div>';
?>
</section>

<br/>
