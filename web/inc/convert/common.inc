<?php

// Test if Admin rights
if (strlen($USER) && $USER !== $ADMINGROUP)	header("Location: ".$APPURL);

// Configuration files
$termsFile = 'conf/config_terms.txt';
$docFile = 'conf/config_doc.txt';

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

# Get documentation for a specific section/term pair
function get_doc($arrdoc, $section, $term='') {
    $termdetail = array();
    $oksection = $okfield = 0;
	foreach($arrdoc as $key => $arr) {
        if (!$oksection && $arr[0]!=='section') continue;
        if (!$oksection && $arr[0] =='section' && $arr[1] !==$section) continue;
        if (!$oksection && $arr[0] =='section' && $arr[1]==$section) {
            if (strlen($term)) { $oksection=1; continue; }
            $termdetail = array('desc' => $arr[2], 'options' => array());
            break;
        }
        if (!$okfield && $arr[0]!=='field') continue;
        if (!$okfield && $arr[0]=='field' && $arr[1] !==$term) continue;
        if (!$okfield && $arr[0]=='field' && $arr[1]==$term) {
            $okfield = 1;
            $termdetail = array('desc' => $arr[2], 'options' => array());
            continue;
        }
        if ($okfield && $arr[0]!=='option') break;
        if ($okfield && $arr[0]=='option') {
            $termdetail['options'][$arr[1]] = $arr[2];
            continue;
        }
    }
    return $termdetail;
}

# Saves the modified terminology definition file and tranferts the modification into the termninology documentation file
function save_and_Finalise ($outTXT) {
	global $termsFile, $docFile, $doctmplFile, $htmlFile, $html2File;

	$ret='';
	try {
		// Write the new Terminology definition file
		file_put_contents($termsFile, $outTXT);

		// Convert Terms to HTML
		$outhtml = tsv2html($termsFile);
		file_put_contents($htmlFile, $outhtml);

		// Update the Terminolog documentation file
		file_put_contents($doctmplFile, tsv2doctmpl($termsFile));
		file_put_contents($docFile, mergeDoc2tmpl($docFile, $doctmplFile));

       	// Convert Docs to HTML
		$outhtml = tsv2html($docFile);
		file_put_contents($html2File, $outhtml);

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

# Selection
$section_select = isset($_GET['section_select']) ? $_GET['section_select'] : '';
$term_select = isset($_GET['term_select']) ? $_GET['term_select'] : '';

# Creation
$section_create = isset($_POST['section_create']) ? $_POST['section_create'] : '';
$term_create = isset($_POST['term_create']) ? $_POST['term_create'] : '';
$term_create = strlen($term_create) && isset($_POST['field']) ? $_POST['field'] : $term_create;

?>
