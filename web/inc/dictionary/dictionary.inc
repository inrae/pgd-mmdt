<?php
#
# Dictionaries
#
# Auteur(s): D.JACOB 
# Copyright: (c) INRAE - 2022-2023
#

# Get the entered user 
$USER = $_SERVER['PHP_AUTH_USER'];

$upload_mode = (strlen($USER) && $USER === 'admin') ? 'inline' : 'none';

# Dictionary name if specified
$DICOSEL='';
if (isset($_GET['dico'])) $DICOSEL=$_GET['dico'];
$dico_file = (isset($_GET['dico'])) ? $cvdir.$DICOSEL.'/'.$DICOSEL.'.txt' : '';

$cvlist=array();  // the corresponding cvlist for each dataverse field if applicable

// Find dictionaries under the cvlist directory
if ($handle = opendir($cvdir)) {
	$html_options='<option value="---">--- Choose a dictionary ---</option>';
	while (false !== ($entry = readdir($handle))) {
		$radical=$cvdir.$entry.'/'.$entry;
		if (file_exists($radical.'.txt') && file_exists($radical.'_format.txt') && file_exists($radical.'.js')) {
			$dico=$entry;
			if (! in_array($dico, $cvlist)) {
				${$dico} = tsvToArray($cvdir.$dico.'/'.$dico.'.txt');
				array_push($cvlist, $dico);
				$selected='';
				if ($dico === $DICOSEL) $selected='selected';
				$html_options .= '<option value="'.$dico.'" '.$selected.'>Information about '.$dico.'</option>';
			}
		}
	}
	closedir($handle);
}

?>

<!DOCTYPE html>
<html lang="en" >

<head>
	<title>Maggot - Dictionary</title>

	<?php echo $HEAD ?>
	<link rel="stylesheet" href="<?php echo $PATH ?>css/ror-widget.css">
	<link rel="stylesheet" href="<?php echo $PATH ?>css/edit.css">
	<script  src="<?php echo $PATH ?>js/typeahead.min.js"></script>
	<script  src="<?php echo $PATH ?>js/ror-typeahead.js"></script>
	<script  src="<?php echo $PATH ?>js/edit.js"></script>

	<script>history.pushState(null, 'Maggot - Dictionary', 'dictionary');</script>
	<script type="text/javascript">
	function getsort(url) {
		window.location.href = url+'&search='+$('#searchterm').val();
	}
	$(document).ready(function() {
		document.getElementById("searchterm" ).addEventListener("input", function(e) {
			select_row( this.value );
		});
	});
	</script>
</head>

<body style="zoom: <? echo $ZOOMWP ?>">

<?php echo $header_title ?>
<br><br>

<!--h1>Dictionaries <a href="<? echo $maggot_doc_url.'dictionaries' ?>" target="_blank"><i class="bi bi-question-octagon-fill" style="color:grey; font-size: 20px"></i></a></h1>
<br-->

<section>
	<form id="formdico" name="formdico" action="#" autocomplete="off" method="get" enctype="multipart/form-data">
		<b>Dictionaries</b>:&nbsp;<a href="<? echo $maggot_doc_url.'dictionaries' ?>" target="doc"><i class="bi-question-octagon-fill" style="color:#1b82c1; font-size: 12px"></i></a>
		<select style="font-size: 1rem;" name="dico" id="dico" class="form-control form-control-sm" onchange="this.form.submit();"><? echo $html_options ?></select>
	</form>
	<br>
	<div style="padding-bottom: 10px;"><div id="errbox" name="errbox" class="msgbox errbox">
		 <div style="margin-left: 2rem; margin-top: 1rem; margin-bottom: 0rem;">
				<span id="errboxmsg">Error !!!</span></p>
		 </div>
	</div></div>
	<table><tr>
	<td><div id="divsearch" class="divsearch" style="padding: 1px; display: none"><b>Search</b>:&nbsp;<input class="searchterm" id="searchterm" value=""></div></td>
	<td>&nbsp;&nbsp;&nbsp;&nbsp;<span id="savemsg" class="savemsg"></span></td>
	</tr></table>
	<div id="tbl-main" name="tbl-main" class="tbl-main tableFixHead"></div>
	<div id="dico-name" style="display: none;"></div>
	<br>
	<div id="tbl-footer" style="display: none;">
		<span style="font-size: small; font-style: italic;">(*) mandatory fields</span>
		<br><br>
		<div>
			<button type="button" class="btn btn-primary btn-xs" onclick="downloadFile('<? echo $dico_file ?>');">Download as a TSV file</button>
			<button type="button" class="btn btn-primary btn-xs" style="display: <? echo $upload_mode ?>;" onclick="$('#uploadDico').click();">Upload a TSV file</button>
			<input type="file" id="uploadDico" style="display: none;" />
		</div>
		<br>
		<div style="padding-bottom: 10px;">
			<div id="errvalid" name="errvalid" class="errvalid">
				<div style="margin-left: 1rem; margin-top: 1rem; margin-bottom: 0rem;"><p><button class="btn btn-errvalid">X</button>&nbsp;&nbsp;<span id="errvalidmsg">Error Validation</span></p></div>
			</div>
		</div>
	</div-->
</section>

<?php

do {

	# No user
	if (strlen($USER)==0) break;

	# No dictionary or unavailable dictionary

	if (! in_array($DICOSEL, $cvlist) || strlen($DICOSEL)==0 ||  $DICOSEL === '---') break;
	// Get the column number to be sorted from the url otherwise keep the default one
	$SORTCOL=1; if (isset($_GET['sort'])) $SORTCOL=$_GET['sort'];

	// Get the search term from the url if specified
	$searchterm=''; if (isset($_GET['search'])) $searchterm=$_GET['search'];

	// Test if Dictionary format file exists
	$format_file=$cvdir.$DICOSEL.'/'.$DICOSEL.'_format.txt';
	if (! is_file($format_file)) {
		$msg='Error: '.$format_file.' does not exist !!!';
		echo "<script>$('#errboxmsg').html('".$msg."'); $('#errbox').css('display','block');</script>";
		break;
	}

	// Read Dictionary format file
	$format = tsvToArray($format_file,true);
	if (! count($format)) {
		$msg='Error: '.$format_file.' is empty !!!';
		echo "<script>$('#errboxmsg').html('".$msg."'); $('#errbox').css('display','block');</script>";
		break;
	}

	// Test if Dictionary file exists
	$dico_file=$cvdir.$DICOSEL.'/'.$DICOSEL.'.txt';
	if (! is_file($dico_file)) {
		$msg='Error: '.$dico_file.' does not exist !!!';
		echo "<script>$('#errboxmsg').html('".$msg."'); $('#errbox').css('display','block');</script>";
		break;
	}

	// Read Dictionary file
	$dico = tsvToArray($dico_file,false);
	if (! count($dico)) {
		$msg='Error: '.$dico_file.' is empty !!!';
		echo "<script>$('#errboxmsg').html('".$msg."'); $('#errbox').css('display','block');</script>";
		break;
	}

	// Test if the number of rows in the format is the same for the number of columns in the dictionary
	$nbrow = count($format);
	$nbcol = array_map("count", $dico)[0];
	if ($nbrow !== $nbcol) {
		$msg='Error: the columns count ('.$nbcol.') of '.$dico_file.' is not the same as the row count ('.$nbrow.') of '.$format_file.' !!!';
		echo "<script>$('#errboxmsg').html('".$msg."'); $('#errbox').css('display','block');</script>";
		break;
	}

	// Sort dictionary
	aasort($dico, $SORTCOL);

	// Build the HTML Table

	// thead
	$var1=$var2=$var3=$var4=''; $sep='';
	$html_thread = '<thead class="thread-light"><tr>';
	$url = basename($_SERVER['SCRIPT_NAME']).'?dico='.$DICOSEL.'&sort=';
	$cnt=0;
	foreach($format as $line) {
		$req=0; $star='';
		if ($line[1] === 'Y') { $req=1; $star=' (*)'; }
		$var1 .= $sep."'".$line[0]."'"; $var2 .= $sep."'".$req."'";  $var3 .= $sep.$line[2];
		$var4 .= (count($line)>3) ? $sep."'".$line[3]."'" : $sep."''" ;
		$cnt++;
		$scol=$cnt; if ($SORTCOL>0 && abs($SORTCOL)==$cnt) $scol=-$cnt;
		$html_thread .= '<th class="tablehead"><a href="javascript:getsort(\\\''.$url.$scol.'\\\');">'.$line[0].'</a>'.$star.'</th>';
		$sep=',';
	}
	if ($USER === 'admin') $html_thread .= '<th class="tablehead tbl-btn" style="width: 140px;"><button class="add">Add new</button></th>';
	$html_thread .= '</tr></thead>';

	// tbody
	$html_tbody ="<tbody id=\"container\">";
	$cnt=0;
	foreach($dico as $line) {
		$html_tbody .="<tr id=\"line".$cnt."\">";
		$col=0;
		foreach($line as $info) {
			if (preg_match('/^http/',$info)) { # URL
				$html_tbody .= '<td class="data" id=\"col'.$col.'\"><a target=\"_blank\" href=\"'.$info.'\">'.$info.'</a></td>';
			} elseif (preg_match('/^\d{4}-\d{4}-\d{4}-\d{3}[0-9|X]$/',$info)) { # ORCID
				$html_tbody .= '<td class="data" id=\"col'.$col.'\"><a target=\"_blank\" href=\"https://orcid.org/'.$info.'\">'.$info.'</a></td>';
			} else {
				$html_tbody .= '<td class="data" id=\"col'.$col.'\">'.$info.'</td>';
			}
			$col++;
		}
		if ($USER === 'admin') $html_tbody .= '<td class="td-btn"><button class="btn save">Save</button><button class="btn edit">Edit</button>&nbsp;<button class="btn delete">Del</button><button class="btn cancel">Cancel</button></td></tr>';
		$cnt++;
	}
	$html_tbody .="</tbody>";

	// If enough elements for the search to be relevant
	if ($cnt>10) $dispsearch='block'; else $dispsearch='none';

	// The full HTML table
	$html_table='<table id="mytable" name="mytable" class="table data" style="margin-bottom: 0rem;">'.$html_thread.$html_tbody.'</table>';

	// Write in the HTML document
	echo "<script>var nameArray = [ ".$var1." ];\nvar requireArray = [ ".$var2." ];\nvar regexArray = [ ".$var3." ];\nvar apiArray = [ ".$var4." ];\nvar nbitems=".$cnt."\n$('#tbl-main').html('".$html_table."'); $('#dico-name').html('".$DICOSEL."');$('#divsearch').css('display','".$dispsearch."');$('#tbl-footer').css('display','block');$('#searchterm').val('".$searchterm."');select_row('".$searchterm."');</script>";

	// If admin user then authorize to upload a file to replace the current dictionary
	if ($USER == 'admin')
		echo "<script>uploadDico.addEventListener('change', function(){ $('#savemsg').html(''); postDico(this.files[0]); })</script>";

} while(0);


?>
