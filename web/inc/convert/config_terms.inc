<?php

// Configuration files
$termsFile = 'conf/config_terms.txt';
$docFile = 'conf/config_doc.txt';
$mdFile = 'docs/doc.md';
$optionFile = "./conf/options.json";

// temporary files
$ID = genKEY();
$doctmplFile = 'tmp/'.$ID.'-2.txt';
$htmlFile = 'tmp/'.$ID.'.html';
$html2File = 'tmp/'.$ID.'-2.html';

# Initialise
$arr_types = array('textbox' => array('autocomplete','min','width'),
                   'areabox' =>  array('rows','cols'),
                   'checkbox' =>  array('open'),
                   'datebox' =>  array('width'),
                   'dropbox' =>  array('width'),
                   'multiselect' =>  array('autocomplete','ws','onto','min'));

$actions = array('Create','Update');


# Get the array for section options
function get_optSection($sect) {
	global $sections;
	$opt_sections = '<option value="">---</option>';
	foreach($sections as $section => $value) {
		$selected = $section == $sect ? 'selected' : '';
		$opt_sections .= '<option value="'.$section.'"'.$selected.'>'.$section.'</option>';
	}
	return $opt_sections;
}

# Get the term options array for a specific section
function get_optTerms($section, $term_select) {
	global $sections;
	$terms = $sections[$section];
	$opt_terms='<option value="">---</option>';
	foreach($terms as $term) {
		$selected = $term == $term_select ? 'selected' : '';
		$opt_terms .= '<option value="'.$term.'"'.$selected.'>'.$term.'</option>';
	}
	return $opt_terms;
}

# Saves the modified terminology definition file and tranferts the modification into the termninology documentation file
function save_and_Finalise ($outTXT) {
	global $termsFile, $docFile, $mdFile, $doctmplFile, $htmlFile, $html2File, $optionFile;

	$ret='';
	try {
		// Write the new Terminology definition file
		file_put_contents($termsFile, $outTXT);

		// Convert TSV to HTML
		$outhtml = tsv2html($termsFile);
		file_put_contents($htmlFile, $outhtml);

		// Convert TSV to a template for documentation
		file_put_contents($doctmplFile, tsv2doctmpl($termsFile));
		file_put_contents($docFile, mergeDoc2tmpl($docFile, $doctmplFile));

		// Convert TSV to MD
		$options = json_decode(file_get_contents($optionFile), NULL, 512, JSON_OBJECT_AS_ARRAY);
		$checkboite=$options['checkboite'];
		$outmd = tsv2md($docFile);
		file_put_contents($mdFile, $outmd);
		file_put_contents($html2File, tsv2html($docFile));
	} catch (Exception $e) {
		$ret = $e->getMessage();
	}
	return $ret;
}


# Read the terminology definition file
$arrterms = tsvToArray($termsFile, $header=true);

# Initialise some arrays (dictionaries) : $sections and $terms
$sections = array();
$terms = array();
foreach($arrterms as $arr) {
	$section = trim($arr[1]);
	$field = trim($arr[0]);
	if (! isset($sections[$section])) $sections[$section] = array();
	array_push($sections[$section], $field);
	array_shift($arr);
	$terms[$field] = $arr;
}

# Section creation / selection values
$section_create = isset($_POST['section_create']) ? $_POST['section_create'] : '';
$section_select = strlen($section_create)==0 && isset($_GET['section_select']) ? $_GET['section_select'] : '';

# Term creation / selection values
$term_create = isset($_POST['term_create']) ? $_POST['term_create'] : '';
$term_create = strlen($term_create) && isset($_POST['field']) ? $_POST['field'] : $term_create;
$term_select = strlen($term_create)==0 && isset($_GET['term_select']) ? $_GET['term_select'] : '';

?>

<!DOCTYPE html>
<html lang="en" >

<head>
	<title>Maggot - Config</title>
	<?php echo $HEADMAIN ?>

	<script type="text/javascript">
	var section_select = "<? echo isset($_GET['section_select']) ? $_GET['section_select'] : '' ?>";
	var section_create = "<? echo isset($_POST['section_create']) ? $_POST['section_create'] : '' ?>";
	var term_select = "<? echo isset($_GET['term_select']) ? $_GET['term_select'] : '' ?>";
	var term_create = "<? echo isset($_POST['term_create']) ? $_POST['term_create'] : '' ?>";

	hidediv = function(div) { document.getElementById(div).style.display = "none"; }
	showdiv = function(div) { document.getElementById(div).style.display = "inline-block"; }

	sectionChange = function() {
		var arr = document.getElementById('section_select') ;
		if (arr.value) 
			window.open("?section_select="+arr.value, "_self");
		else
			window.open("?", "_self");
	}

	termChange = function() {
		var arr = document.getElementById('term_select') ;
		if (arr.value) 
			window.open("?section_select="+section_select+"&term_select="+arr.value, "_self");
		else
			window.open("?section_select="+section_select, "_self");
	}

	typeChange = function() {
		const types = ["checkbox", "dropbox"];
		var arr = document.getElementById('type') ;
		if (types.includes(arr.value)) 
			showdiv('listdiv');
		else
			hidediv('listdiv');
	}

	clickRemove = function() {
		$('#lblconfirm').text('Are you sure you want to delete this term ?');
		$('#action').val('Remove');
		showdiv('confirmation');
	}

	clickAction = function(value) {
		$('#lblconfirm').text('Are you sure you want to update the terminoly definition file ?');
		$('#action').val(value);
		showdiv('confirmation');
	}

	clickReset = function() {
		window.open("?", "_self");
	}

	$(document).ready(function() {
		if (!section_select && ! section_create) { showdiv('d1'); showdiv('d2'); showdiv('d3'); showdiv('d4'); }
		if ( section_select && ! section_create) { showdiv('d1'); hidediv('d2'); hidediv('d3'); hidediv('d4'); }
		if (!section_select &&   section_create) { hidediv('d1'); hidediv('d2'); showdiv('d3'); hidediv('d4'); }
		if (document.getElementById('d5') instanceof HTMLElement) {
			hidediv('d5'); hidediv('d6'); hidediv('d7'); hidediv('d8');
		}
		if (section_select) {
			if (!term_select && ! term_create) { showdiv('d5'); showdiv('d6'); showdiv('d7'); showdiv('d8'); }
			if ( term_select && ! term_create) { showdiv('d5'); hidediv('d6'); hidediv('d7'); hidediv('d8'); }
			if (!term_select &&   term_create) { hidediv('d5'); hidediv('d6'); showdiv('d7'); hidediv('d8'); }
		}
	});
	</script>
</head>

<body style="zoom: <? echo $ZOOMWP ?>">

<?php echo $header_config ?>
<br><br>

<h1>Terminology configuration <a href="<? echo $maggot_doc_url.'definitions/terminology/' ?>" target="doc"><i class="bi bi-question-octagon-fill" style="color:grey; font-size: 20px"></i></a></h1>
<br>

<section>
<form id="formulaire" enctype="multipart/form-data" autocomplete="off" action="#" method="post">

<fieldset class="form-group2 fieldset_section">
	<legend class="legend_section">Section</legend>
<center>

<!-- Section -->
<div class="form-group2" style="width:250px" id="d1">
	<span class="labeldiv">Select a Section</span>
	<fieldset>
		<select id="section_select" name="section_select" class="form-control form-control-sm section" onchange="sectionChange()"><? echo get_optSection($section_select) ?></select>
	</fieldset>
</div>
<div class="form-group2" id="d2"><span class="blue">&nbsp;|&nbsp;</span></div>
<div class="form-group2 " style="width:250px" id="d3">
	<span class="labeldiv">Create a new Section</span>
	<input class="form-control form-control-sm solo" type="text" name="section_create" id="section_create" style="width:250px" value="<? echo $section_create ?>">
</div>
<div class="form-group2" id="d4">
	<input type="submit" value="Add" class="btn btn-primary" style="width: 50px;">
</div>

<?php
if (strlen($section_select)) {
	echo '
<!-- Term -->
<div class="form-group2" style="width:250px" id="d5">
	<span class="labeldiv">Select a Term</span>
	<fieldset>
		<select id="term_select" name="term_select" class="form-control form-control-sm section" onchange="termChange()">'.get_optTerms($section_select, $term_select).'</select>
	</fieldset>
</div>
<div class="form-group2" id="d6"><span class="blue">&nbsp;|&nbsp;</span></div>
<div class="form-group2 " style="width:250px" id="d7">
	<span class="labeldiv">Create a new Term</span>
	<input class="form-control form-control-sm solo" type="text" name="term_create" id="term_create" style="width:250px" value="'.$term_create.'">
</div>
<div class="form-group2" id="d8">
	<input type="submit" value="Add" class="btn btn-primary" style="width: 50px;">
</div>';
}
?>

</center>
</fieldset>

<?php
$section = $section_create ? $section_create : $section_select;

$infos = array($section, 'N', 'N', '', '', '', '', '');

if (strlen($term_select)) {
	$term = $term_select;
	$infos = $terms[$term];
	$term_create = '';
}

if (isset($_POST['action']) && in_array($_POST['action'], $actions)) {
	if ($_POST['action']==$actions[0]) $term_create = $_POST['field'];
	$required = isset($_POST['required']) ? $_POST['required'] : $infos[1];
	$search = isset($_POST['search']) ? $_POST['search'] : $infos[2];
	$shortview = isset($_POST['shortview']) ? $_POST['shortview'] : $infos[3];
	$seltype = isset($_POST['type']) ? $_POST['type'] : $infos[4];
	$features = isset($_POST['features']) ? $_POST['features'] : $infos[5];
	$label = isset($_POST['label']) ? $_POST['label'] : $infos[6];
	$termlist = isset($_POST['termlist']) ? $_POST['termlist'] : $infos[7];
	$infos = array($section, $required, $search, $shortview, $seltype, $features, $label, $termlist);
}

$term = $term_create ? $term_create : $term_select ;

$r_check_y = $infos[1]=="Y" ? 'checked' : '';
$r_check_n = $infos[1]=="N" ? 'checked' : '';
$s_check_y = $infos[2]=="Y" ? 'checked' : '';
$s_check_n = $infos[2]=="N" ? 'checked' : '';

$opt_types = '<option value="">---</option>';
foreach($arr_types as $type => $value) {
	$selected = $type == $infos[4] ? 'selected' : '';
	$opt_types .= '<option value="'.$type.'"'.$selected.'>'.$type.'</option>';
}

// Term Definition Form
if (strlen($section_create) || strlen($term_select) || strlen($term_create)) {
	echo '
<fieldset class="form-group2 fieldset_section">
    <legend class="legend_section">Term</legend>

<div class="form-group2" style="width:200px">
    <span class="labeldiv">Field name</span>
    <input class="form-control form-control-sm solo" type="text" name="field" id="field" style="width:200px" value="'.$term.'">
</div>
&nbsp;
<div class="form-group2" style="width:200px">
    <span class="labeldiv">Field Label</span>
    <input class="form-control form-control-sm solo" type="text" name="label" id="label" style="width:200px" value="'.$infos[6].'">
</div>
&nbsp;&nbsp;
<div class="form-group2">
    <span class="labeldiv">Short View</span>
    <input class="form-control form-control-sm solo" type="text" name="shortview" id="shortview" style="width:200px" value="'.$infos[3].'">
</div>
&nbsp;&nbsp;
<div class="form-group2">
    <span class="labeldiv">Required</span>&nbsp;
    <input type="radio" id="req1" name="required" value="Y" '.$r_check_y.'>&nbsp;<label for="req1">Yes</label> &nbsp;&nbsp;
    <input type="radio" id="req2" name="required" value="N" '.$r_check_n.'>&nbsp;<label for="req2">No</label> &nbsp;&nbsp;
</div>
&nbsp;&nbsp;
<div class="form-group2">
    <span class="labeldiv">Search</span>&nbsp;
    <input type="radio" id="search1" name="search" value="Y" '.$s_check_y.'>&nbsp;<label for="search1">Yes</label> &nbsp;&nbsp;
    <input type="radio" id="search2" name="search" value="N" '.$s_check_n.'>&nbsp;<label for="search2">No</label> &nbsp;&nbsp;
</div>
<br><br>
<div class="form-group2" style="width:200px">
	<span class="labeldiv">Type</span>
	<fieldset>
		<select id="type" name="type" class="form-control form-control-sm section" onchange="typeChange()">'.$opt_types.'</select>
	</fieldset>
</div>
&nbsp;
<div class="form-group2">
    <span class="labeldiv">Features</span>
    <input class="form-control form-control-sm solo" type="text" name="features" id="features" style="width:1050px" value="'.$infos[5].'">
</div>';
	$display = in_array($infos[4], array('checkbox', 'dropbox')) ? "inline-block" : "none";
	echo '
<div class="form-group2" id="listdiv" style="display:'.$display.'">
	<br>
    <span class="labeldiv">Predefined terms</span>
    <input class="form-control form-control-sm solo" type="text" name="termlist" id="termlist" style="width:1260px" value="'.$infos[7].'">
</div>';

	$opt_after = '<option value="after" selected>After</option><option value="before">Before</option>';
	$opt_before = '<option value="after">After</option><option value="before" selected>Before</option>';
	$opt_insert = isset($_POST['insert']) && $_POST['insert'] == 'before' ? $opt_before : $opt_after ;
	$section_insert = isset($_POST['section_insert']) ? $_POST['section_insert'] : '' ;
	$term_insert = isset($_POST['term_insert']) ? $_POST['term_insert'] : '' ;

	if (strlen($section_create))
		echo '
<div class="form-group2" style="width:200px">
	<br>
    <span class="labeldiv">Insert</span>
	<fieldset>
		<select id="insert" name="insert" class="form-control form-control-sm section">'.$opt_insert.'</select>
	</fieldset>
</div>
&nbsp;
<div class="form-group2" style="width:250px">
	<span class="labeldiv">Section</span>
	<fieldset>
		<select id="section_insert" name="section_insert" class="form-control form-control-sm section">'.get_optSection($section_insert).'</select>
	</fieldset>
</div>';

	if (! strlen($section_create) && strlen($term_create))
		echo '
<div class="form-group2" style="width:200px">
	<br>
	<span class="labeldiv">Insert</span>
	<fieldset>
		<select id="insert" name="insert" class="form-control form-control-sm section">'.$opt_insert.'</select>
	</fieldset>
</div>
&nbsp;
<div class="form-group2" style="width:250px">
	<span class="labeldiv">Term</span>
	<fieldset>
		<select id="term_insert" name="term_insert" class="form-control form-control-sm section">'.get_optTerms($section_select, $term_insert).'</select>
	</fieldset>
</div>';

// Buttons
	$action = strlen($section_create) || strlen($term_create) ? $actions[0] : $actions[1] ;
	echo '
<br><br>
<div class="form-group2" id="action1">
	<button type="button" class="btn btn-primary" style="width: 150px;" onclick="clickAction(\''.$action.'\');">'.$action.'</button>
</div>';

	if (strlen($term_select) && !in_array($term_select, $required_fields))
		echo '
<div class="form-group2" id="action2">
	<button type="button" id="remove" name="remove" class="btn btn-danger" style="width: 150px;" onclick="clickRemove();">Remove</button>
</div>';

	echo '
<div class="form-group2" id="action3" style="display:none">
	<button type="button" id="reset" name="reset" class="btn btn-primary" style="width: 150px;" onclick="clickReset();">Reset</button>
</div>

</fieldset>';

// Confirmation
	echo '<div id="confirmation" style="display:none">
<span style="color:#52499e" id="lblconfirm"></span><input type="hidden" id="action" name="action" value="">
&nbsp;<input type="submit" id="confirm" name="confirm" value="Yes" class="btn btn-primary" style="width: 50px;">
&nbsp;<button type="button" id="cancel" name="cancel" class="btn btn-primary" style="width: 50px;" onclick="hidediv(\'confirmation\');">No</button>
</div>';

}

// Message Boxes
echo '</form>
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
			<span>CAUTION: Don\'t forget to restart the Maggot application in command line mode on the server to take into account the modification in the terminology definition file</span></p>
		</div>
	</div></div>
</div>';

//echo "<pre>"; print_r($sections); echo "</pre>";
//echo "<pre>"; print_r($infos); echo "</pre>";
//echo "<pre>"; print_r($terms); echo "</pre>";
//echo "<pre>"; print_r($_POST); echo "</pre>";

do
{
	if (! isset($_POST['action'])) break;

	$action_post = $_POST['action'];


	// Confirmation : Create / Update a Term into a (New) Section
	if (isset($_POST['confirm']) && $_POST['confirm']=='Yes' && in_array($action_post, $actions))
	{
		//echo "<pre>"; print_r($_POST); echo "</pre>";


	// == Do some checking ==

		// Check if empty
		$errormsg = '';
		$arr_empty = array();
		if (!strlen($term)) array_push($arr_empty, 'Field name');
		if (!strlen($label)) array_push($arr_empty, 'Field Label');
		if (!strlen($features) and $seltype !== 'textbox') array_push($arr_empty, 'Features');
		if (!strlen($seltype)) array_push($arr_empty, 'Type');
		if (count($arr_empty)>0)
			$errormsg = 'Some fields are not defined : <i>'.implode(', ', $arr_empty).'</i>';

		// Check Field name
		if (!strlen($errormsg) && strlen($term)>0) {
			if ($action_post==$actions[0] && in_array($term, array_keys($terms)))
				$errormsg = 'The field name <i>'.$term.'</i> is already in use';
			elseif (! preg_match('/^[a-zA-Z0-9_]+$/', $term) || ! preg_match('/^[a-zA-Z]/', $term))
				$errormsg = 'Only letters, numbers, and underscores are allowed for the field name and it must begin with a letter';
			elseif (strlen($term)>20)
				$errormsg = 'The number of characters is limited to 20 for the field name';
		}

		// Check Field label
		if (!strlen($errormsg) && strlen($label)>0) {
			if (strlen($label)>40)
				$errormsg = 'The number of characters is limited to 20 for the field name';
			elseif (preg_match('/[@={}~#&\"\'%$!:;,\\[\\]]/',$label))
				$errormsg = 'the field name contains one or more special characters.';
		}

		// Check Shortview
		if (!strlen($errormsg) && strlen($shortview)>0) {
			if (!ctype_digit($shortview))
				$errormsg = '<i>Short View</i> must contain only digits';
		}

		// Check Type & Features
		if (!strlen($errormsg) && strlen($seltype)>0) {
			$diff = array_diff(extractNames($features), $arr_types[$seltype]);
			$arr = assocPars($features);
			if (! empty($diff))
				$errormsg = 'Some parameters in the features field do not match those expected for a <i>"'.$seltype.'"</i> type field: <i>'.implode(', ', $diff).'</i>';
			elseif ($seltype=='checkbox' && preg_match('/open=0/', $features) && !strlen($termlist))
				$errormsg = '<i>Predefined terms</i> must contain certain terms separated by a comma in case of <i>checkbox</i> type with a closed term list (<i>open=0</i>)';
			elseif ($seltype=='checkbox' && (!isset($arr['open']) || !in_array($arr['open'], array(0,1))))
				$errormsg = 'A <i>checkbox</i> type must have a feature <i>open</i> equal to 0 or 1';
			elseif (!isset($arr['autocomplete']) && (isset($arr['ws']) || isset($arr['onto'])))
				$errormsg = '<i>ws</i> and <i>onto</i> features need to specify the <i>autocomplete</i> one';
			elseif (isset($arr['autocomplete']) && $arr['autocomplete']=='typeahead' && ( !isset($arr['ws']) || !isset($arr['onto'])))
				$errormsg = '<i>autocomplete=typeahead</i> implies to specify <i>ws</i> and <i>onto</i> features';
			elseif (isset($arr['onto']) && preg_match('/[,; ]/', $arr['onto']))
				$errormsg = 'the separator for the <i>onto</i> feature is the colon (:)';
			elseif (isset($arr['rows']) && !ctype_digit($arr['rows']))
				$errormsg = 'the <i>rows</i> feature must contain only digits';
			elseif (isset($arr['cols']) && !ctype_digit($arr['cols']))
				$errormsg = 'the <i>cols</i> feature must contain only digits';
			elseif (isset($arr['min']) && !ctype_digit($arr['min']))
				$errormsg = 'the <i>min</i> feature must contain only digits';
			elseif (isset($arr['width']) && ! preg_match('/^[0-9]{2,3}px$/', $arr['width']))
				$errormsg = 'the <i>width</i> feature must correspond to a numeric value (>=10 && <1000) followed without space by \'px\'';
			elseif (isset($arr['autocomplete']) && ! preg_match('/^[a-zA-Z0-9_]+$/', $arr['autocomplete']))
				$errormsg = 'Only letters, numbers, and underscores are allowed for the <i>autocomplete</i> feature';
		}

		// Check Predefined terms
		if (!strlen($errormsg) && strlen($termlist)>0) {
			if (preg_match('/[@={}()~#&\"\'%$!:;\\[\\]]/',$termlist))
				$errormsg = 'the Predefined terms contains one or more special characters.';
		}

		// Check insert section
		if (!strlen($errormsg) && strlen($section_create) && !strlen($section_insert))
			$errormsg = 'You must choose a section to insert the new one';

		// Check insert term
		if (!strlen($errormsg) && strlen($section_select) && strlen($term_create) && !strlen($term_insert))
			$errormsg = 'You must choose a term to insert the new one';

		// Come out if some errors
		if (strlen($errormsg)) {
			$htmloutput="ERROR: ".addslashes($errormsg);
			echo "<script>$('#errbox').css('display','block'); $('#errboxmsg').html('".$htmloutput."');</script>";
			break;
		}


	// == Insert / Update the Term into the Terminology definition file ==

		// New line to insert / update
		$newline = implode("\t", array($term,$section,$required,$search,$shortview,$seltype,$features,$label,$termlist));

		// Defined the 3 following conditions :
		$cond1 = $action_post==$actions[1] && strlen($section_select) && strlen($term_select);
		$cond2 = $action_post==$actions[0] && strlen($section_select) && strlen($term_create);
		$cond3 = $action_post==$actions[0] && strlen($section_create);

		// Init some variables
		$outTXT = '';
		$insert = isset($_POST['insert']) ? $_POST['insert'] : 'after';
		$section_after = 0;
		$ok_newline = 0;

		do {
			$handle = fopen($termsFile, "r");
			if (! $handle) break;

			// Content
			while (($line = fgets($handle)) !== false) {
				$outLine = $line;
				$val = explode("\t", $line);

				// 1 - update a term
				if ($cond1 && $term==$val[0] && $section==$val[1]) {
					$outLine = $newline."\n";
					$ok_newline = 1;
				}

				// 2 - create a new term into a section
				if ($cond2 && $section==$val[1] && $term_insert==$val[0]) {
					if ($insert=='before') $outLine = $newline."\n".$line;
					else                   $outLine = $line.$newline."\n";
					$ok_newline = 1;
				}

				// 3 - create a new section along with a new section
				if ($cond3 && $section_insert==$val[1] && $insert=='before') {
					$outLine = $newline."\n".$line;
					$ok_newline = 1;
				}

				if ($cond3 && $section_insert==$val[1] && $insert=='after') {
					$section_after = 1;
				}

				if ($cond3 && $section_insert!=$val[1] && $insert=='after' && $section_after) {
					$outLine = $newline."\n".$line;
					$section_after = 0;
					$ok_newline = 1;
				}

				$outTXT .= $outLine;
			}
			fclose($handle);

			if (!$ok_newline && strlen($section_create)) {
				$outLine = $newline."\n";
				$ok_newline = 1;
			}

			// if condition was not found to insert / update the new line
			if (!$ok_newline) {
				$errormsg = 'Conditions was not found to insert / update the new line !?';
				$htmloutput="ERROR: ".addslashes($errormsg);
				echo "<script>$('#errbox').css('display','block'); $('#errboxmsg').html('".$htmloutput."');</script>";
				break;
			}

			$ret = save_and_Finalise ($outTXT);
			if (strlen($ret)) {
				$htmloutput="ERROR: ".addslashes($ret);
				echo "<script>$('#errbox').css('display','block'); $('#errboxmsg').html('".$htmloutput."');</script>";
				break;
			}

			$successmsg = 'the new line has been insert / update. View the <a href="'.$PATH.$htmlFile.'" target="_blank">terminology definition file</a> and the <a href="'.$PATH.$html2File.'" target="_blank">terminology documentation file</a>';
			$htmloutput="SUCESS: ".addslashes($successmsg);
			echo "<script>$('#successresp').css('display','block'); $('#successmsg').html('".$htmloutput."'); $('#caution').css('display','block'); $('#action1').css('display','none'); $('#action2').css('display','none'); $('#confirmation').css('display','none'); $('#action3').css('display','inline-block');</script>";

		}  while(0);
		break;
	}


	// == Remove a Term into a Section ==

	if (isset($_POST['confirm']) && $_POST['confirm']=='Yes' && $action_post=='Remove')
	{
		//echo "<pre>"; print_r($_POST); echo "</pre>";

		// Init some variables
		$outTXT = '';
		$ok_rmline = 0;

		do {
			$handle = fopen($termsFile, "r");
			if (! $handle) break;

			// Content
			while (($line = fgets($handle)) !== false) {
				$outLine = $line;
				$val = explode("\t", $line);

				// Remove term if found
				if ($term==$val[0] && $section==$val[1]) {
					$ok_rmline = 1;
				} else {
					$outTXT .= $outLine;
				}

			}
			fclose($handle);

			// if condition was not found to insert / update the new line
			if (!$ok_rmline) {
				$errormsg = 'Conditions was not found to remove the field !?';
				$htmloutput="ERROR: ".addslashes($errormsg);
				echo "<script>$('#errbox').css('display','block'); $('#errboxmsg').html('".$htmloutput."');</script>";
				break;
			}

			$ret = save_and_Finalise ($outTXT);
			if (strlen($ret)) {
				$htmloutput="ERROR: ".addslashes($ret);
				echo "<script>$('#errbox').css('display','block'); $('#errboxmsg').html('".$htmloutput."');</script>";
				break;
			}

			$successmsg = 'the field <i>'.$term.'</i> has been remove from the section <i>'.$section.'</i>. View the <a href="'.$PATH.$htmlFile.'" target="_blank">terminology definition file</a>';
			$htmloutput="SUCESS: ".addslashes($successmsg);
			echo "<script>$('#successresp').css('display','block'); $('#successmsg').html('".$htmloutput."'); $('#caution').css('display','block'); $('#action1').css('display','none'); $('#action2').css('display','none'); $('#confirmation').css('display','none'); $('#action3').css('display','inline-block');</script>";

		}  while(0);
		break;
	}

} while(0);
