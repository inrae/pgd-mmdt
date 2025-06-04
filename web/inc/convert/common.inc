<?php

// Test if Admin rights
if (strlen($USER) && $USER !== $ADMINGROUP)	header("Location: ".$APPURL);

// Configuration files
$termsFile = 'conf/config_terms.txt';
$docFile = 'conf/config_doc.txt';

// temporary files
$ID = genKEY();
$doctmplFile = 'tmp/doc-'.$ID.'.txt';
$htmlFile = 'tmp/terms-'.$ID.'.html';
$html2File = 'tmp/doc-'.$ID.'.html';

# Initialise
$arr_types = array('textbox' => array('autocomplete','min','width'),
                   'areabox' =>  array('rows','cols'),
                   'checkbox' =>  array('open'),
                   'datebox' =>  array('width'),
                   'dropbox' =>  array('width'),
                   'multiselect' =>  array('autocomplete','ws','onto','min'));

$actions = array('Create','Update');

# Get the HTML select options
function get_HTML_options($arr, $val_select)
{
	$opt_terms='<option value="">---</option>';
	foreach($arr as $value) {
		$selected = $value == $val_select ? 'selected' : '';
		$opt_terms .= '<option value="'.$value.'"'.$selected.'>'.$value.'</option>';
	}
	return $opt_terms;
}

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
function get_optTerms($section, $term_select)
{
	global $sections;
	return get_HTML_options($sections[$section], $term_select);
}

# Get documentation for a specific section/term pair
function get_doc($arrdoc, $section, $term='')
{
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

# Get the list of dictionnaries
function get_dico_array()
{
	global $cvdir;
	$arr_dico = array();
	// Find dictionaries under the cvlist directory
	if ($handle = opendir($cvdir)) {
		while (false !== ($entry = readdir($handle))) {
			$radical=$cvdir.$entry.'/'.$entry;
			if (file_exists($radical.'.txt') && file_exists($radical.'_format.txt') && file_exists($radical.'.js'))
				array_push($arr_dico, $entry);
		}
	}
	closedir($handle);
	return $arr_dico;
}

# Get the list of autocomplete scripts based on whether it is typeahead or not
function get_vocab_array($typeahead=0)
{
	$jsdir = 'js/autocomplete';
	$arr_vocab = array();
	// Find vocabularies under the vocab directory
	if ($handle = opendir($jsdir)) {
		while (false !== ($entry = readdir($handle))) {
			$path=$jsdir.'/'.$entry;
			if (! file_exists($path) || ! preg_match('/.js$/', $entry)) continue;
			$entry = str_replace('.js', '', $entry);
			if ($typeahead && $entry !== 'template_typeahead') {
				if (preg_match('/_typeahead/', $entry))
					array_push($arr_vocab, str_replace('_typeahead', '', $entry));
			} else {
				if (!preg_match('/typeahead/', $entry))
					array_push($arr_vocab, $entry);
			}
		}
	}
	closedir($handle);
	return $arr_vocab;
}

# Saves the modified terminology definition file and tranferts the modification into the termninology documentation file
function save_and_Finalise ($outTXT)
{
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
