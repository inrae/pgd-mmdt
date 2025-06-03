<?php 
require_once('inc/convert/common.inc');

do
{
	if (!isset($_POST['confirm']) || ! $_POST['confirm']=='Yes') break;
	if (!strlen($section_select)) break;

	# Read the terminology documentation file
	$arrdoc = tsvToArray($docFile, $header=true);
	$termdesc = get_doc($arrdoc, $section_select, $term_select);
	if (!isset($termdesc['desc'])) break;

	// Update Confirmation
	if (!strlen($term_select)) {
		$type = 'section'; $name = $section_select;
		$newlines = "section\t".$section_select."\t".$_POST['desc']."\n";
	} else {
		$type = 'field'; $name = $term_select;
		$newlines = "field\t".$term_select."\t".$_POST['desc']."\n";
	}

	$cnt = isset($_POST['optcnt']) && $_POST['optcnt']>0 ? $_POST['optcnt'] : 0 ;

	if ($cnt>0 && count($termdesc['options'])==$cnt) {
		$id=0;
		foreach($termdesc['options'] as $key => $value) {
			$newlines .= "option\t".$key."\t".$_POST['option'.$id]."\n";
			$id++;
		}
	}
	//echo "<pre>"; print_r($newlines); echo "</pre>";

	// Init some variables
	$outTXT = '';
	$ok_line = 0;

	$handle = fopen($docFile, "r");
	if (! $handle) break;

	// Content
	while (($line = fgets($handle)) !== false) {
		$outLine = $line;
		$val = explode("\t", $line);

		// if found the corresponding field, replace it
		if ($type==$val[0] && $name==$val[1]) {
			$outLine = $newlines;
			$ok_line = 1;
			// Ignore next lines if they contain 'option'
			while (($l = fgets($handle)) !== false) {
				$val = explode("\t", $l);
				if ($val[0] !== 'option') { $outLine .= $l; break; }
			}
		}
		$outTXT .= $outLine; 
	}
	fclose($handle);

	// Save into the terminology documentation file
	file_put_contents($docFile, $outTXT);

	// Convert TSV to HTML
	file_put_contents($html2File, tsv2html($docFile));

} while(0);

?>


<!DOCTYPE html>
<html lang="en" >

<head>
	<title>Maggot - Config</title>
	<?php echo $HEADMAIN ?>

	<script type="text/javascript">
	var section_select = "<? echo isset($_GET['section_select']) ? $_GET['section_select'] : '' ?>";
	var term_select = "<? echo isset($_GET['term_select']) ? $_GET['term_select'] : '' ?>";

	hidediv = function(div) { document.getElementById(div).style.display = "none"; }
	showdiv = function(div) { document.getElementById(div).style.display = "inline-block"; }

	sectionChange = function() {
		var arr = document.getElementById('section_select') ;
		if (arr.value) 
			window.open("?docs&section_select="+arr.value, "_self");
		else
			window.open("?docs", "_self");
	}

	termChange = function() {
		var arr = document.getElementById('term_select') ;
		if (arr.value) 
			window.open("?docs&section_select="+section_select+"&term_select="+arr.value, "_self");
		else
			window.open("?docs&section_select="+section_select, "_self");
	}

	clickUpdate = function() {
		$('#lblconfirm').text('Are you sure you want to update the terminoly documentation file ?');
		showdiv('confirmation');
	}

	</script>
</head>

<body style="zoom: <? echo $ZOOMWP ?>">

<?php echo $header_config ?>
<br><br>

<h1>Terminology documentation <a href="<? echo $maggot_doc_url.'definitions/terminology/' ?>" target="doc"><i class="bi bi-question-octagon-fill" style="color:grey; font-size: 20px"></i></a></h1>
<br>

<section>
<form id="formulaire" enctype="multipart/form-data" autocomplete="off" action="#" method="post">

<fieldset class="form-group2 fieldset_section">
	<legend class="legend_section"><span style="background-color:white">&nbsp;&nbsp;Selection&nbsp;&nbsp;</span></legend>
<center>

<!-- Section -->
<div class="form-group2" style="width:250px" id="d1">
	<span class="labeldiv">Select a Section</span>
	<fieldset>
		<select id="section_select" name="section_select" class="form-control form-control-sm section" onchange="sectionChange()"><? echo get_optSection($section_select) ?></select>
	</fieldset>
</div>

<?php
if (strlen($section_select))
	echo '
<!-- Term -->
<div class="form-group2" style="width:250px" id="d5">
	<span class="labeldiv">Select a Term</span>
	<fieldset>
		<select id="term_select" name="term_select" class="form-control form-control-sm section" onchange="termChange()">'.get_optTerms($section_select, $term_select).'</select>
	</fieldset>
</div>';
?>

</center>
</fieldset>

<?php
// Term Documentation Form
do {
	if (!strlen($section_select)) break;

	# Read the terminology documentation file
	$arrdoc = tsvToArray($docFile, $header=true);
	$termdesc = get_doc($arrdoc, $section_select, $term_select);
	if (!isset($termdesc['desc'])) break;
	
	echo '
<fieldset class="form-group2 fieldset_section">
    <legend class="legend_section"><span style="background-color:white">&nbsp;&nbsp;Documentation&nbsp;&nbsp;</span></legend>
	<input type="hidden" id="section" name="section" value="'.$section_select.'">
	<input type="hidden" id="term" name="term" value="'.$term_select.'">';

	echo '
<div class="form-group2" style="width:100%">
	<span class="labeldiv">Description</span>
	<fieldset>
		<textarea class="form-control form-control-sm" rows="3" cols="30" id="desc" name="desc" style="width:100%">'.$termdesc['desc'].'</textarea>
	</fieldset>
</div>';

	// Options
	$cnt=0;
	if (!empty($termdesc['options']))
		foreach($termdesc['options'] as $key => $value) {
			echo '
<br><br>
<div class="form-group2" style="width:100%">
	<span class="labeldiv">Option "'.$key.'"</span>
	<fieldset>
		<textarea class="form-control form-control-sm" rows="3" cols="30" id="option'.$cnt.'" name="option'.$cnt.'" style="width:100%">'.$value.'</textarea>
	</fieldset>
</div>';
			$cnt++;
		}

	// Buttons
	echo '
<input type="hidden" id="optcnt" name="optcnt" value="'.$cnt.'">
<br><br>
<div class="form-group2" id="action1">
	<button type="button" class="btn btn-primary" style="width: 150px;" onclick="clickUpdate();">Update</button>
</div>
</fieldset>
<div id="confirmation" style="display:none">
<span style="color:#52499e" id="lblconfirm"></span><input type="hidden" id="action" name="action" value="">
&nbsp;<input type="submit" id="confirm" name="confirm" value="Yes" class="btn btn-primary" style="width: 50px;">
&nbsp;<button type="button" id="cancel" name="cancel" class="btn btn-primary" style="width: 50px;" onclick="hidediv(\'confirmation\');">No</button>
</div>';

} while(0);


?>

</form>
</section>
<br>
<div id="result" style="display: block">
	<div style="padding-bottom: 10px;"><div id="errbox" name="errbox" class="msgbox errbox">
		<div style="margin-left: 2rem; margin-top: 1rem; margin-bottom: 0rem;">
			<span id="errboxmsg">Error !!!</span></p>
		</div>
	</div></div>
	<div style="padding-bottom: 10px;"><div id="successresp" name="successresp" class="msgbox successbox">
		<div style="margin-left: 2rem; margin-top: 1rem; margin-bottom: 0rem;">
			<span id="successmsg">Success !!!</span></p>
		</div>
	</div></div>
	<div style="padding-bottom: 10px;"><div id="caution" name="caution" class="msgbox cautionbox">
		<div style="margin-left: 2rem; margin-top: 1rem; margin-bottom: 0rem;">
			<span>CAUTION: Don't forget to restart the Maggot application in command line mode on the server to take into account the modification in the terminology documentation file</span></p>
		</div>
	</div></div>
</div>

<?php

if (isset($_POST['confirm']) && $_POST['confirm']=='Yes') {

	if (isset($ok_line) && $ok_line==1) {
		$successmsg = 'the new line has been insert / update. View the <a href="'.$PATH.$html2File.'" target="_blank">terminology documentation file</a>';
		$htmloutput="SUCESS: ".addslashes($successmsg);
		echo "<script>$('#successresp').css('display','block'); $('#successmsg').html('".$htmloutput."'); $('#caution').css('display','block'); $('#action1').css('display','none'); $('#action2').css('display','none'); $('#confirmation').css('display','none'); $('#action3').css('display','inline-block');</script>";

	} 
}
?>