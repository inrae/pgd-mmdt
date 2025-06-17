<?php

// Get the different dictionnaries from options / terms
if (file_exists("./conf/options.json"))
	$options = json_decode(file_get_contents("./conf/options.json"), NULL, 512, JSON_OBJECT_AS_ARRAY);
else
	$options = json_decode(file_get_contents("./conf/config_terms.json"), NULL, 512, JSON_OBJECT_AS_ARRAY);
$listdico=$options['listdico'];
$chkbxdico=$options['chkbxdico'];

// Get the different dictionnaries from config_terms.json
$options = json_decode(file_get_contents("./conf/config_terms.json"), NULL, 512, JSON_OBJECT_AS_ARRAY);
$section=$options['sections'];
$section_search=$options['sections_search'];
$required=$options['required'];
$shortview=$options['shortview'];
$checkboite=$options['checkboite'];
$listboite=$options['listboite'];
$multiboite=$options['multiboite'];
$txtboite=$options['txtboite'];
$areaboite=$options['areaboite'];
$dateboite=$options['dateboite'];

// minimum required fields
$required_fields = array('title', 'description');

// accented characters
$accented_chr = array(
	'Š'=>'S', 'š'=>'s', 'Ž'=>'Z', 'ž'=>'z', 'À'=>'A', 'Á'=>'A', 'Â'=>'A', 'Ã'=>'A', 'Ä'=>'A', 'Å'=>'A', 'Æ'=>'A', 'Ç'=>'C', 'È'=>'E', 'É'=>'E',
	'Ê'=>'E', 'Ë'=>'E', 'Ì'=>'I', 'Í'=>'I', 'Î'=>'I', 'Ï'=>'I', 'Ñ'=>'N', 'Ò'=>'O', 'Ó'=>'O', 'Ô'=>'O', 'Õ'=>'O', 'Ö'=>'O', 'Ø'=>'O', 'Ù'=>'U',
	'Ú'=>'U', 'Û'=>'U', 'Ü'=>'U', 'Ý'=>'Y', 'Þ'=>'B', 'ß'=>'Ss', 'à'=>'a', 'á'=>'a', 'â'=>'a', 'ã'=>'a', 'ä'=>'a', 'å'=>'a', 'æ'=>'a', 'ç'=>'c',
	'è'=>'e', 'é'=>'e', 'ê'=>'e', 'ë'=>'e', 'ì'=>'i', 'í'=>'i', 'î'=>'i', 'ï'=>'i', 'ð'=>'o', 'ñ'=>'n', 'ò'=>'o', 'ó'=>'o', 'ô'=>'o', 'õ'=>'o',
	'ö'=>'o', 'ø'=>'o', 'ù'=>'u', 'ú'=>'u', 'û'=>'u', 'ý'=>'y', 'þ'=>'b', 'ÿ'=>'y'
);

# Generate a KEY (30  alpha-num. char. )
function genKEY()
{
	return md5(uniqid(rand(),true));
}

# Executes a database query, then returns an array containing all results
function dbQueryToArray($filter=[], $options=[])
{
	global $client, $database, $collection;
	$query = new MongoDB\Driver\Query($filter, $options);
	$obj = $client->executeQuery($database.'.'.$collection, $query)->toArray();
	return $obj;
}

# Extract the parameter names from a string of type "par1=value1,par2=value2,par3=value3"
function extractNames($string)
{
	$ret = [];
	if (strlen($string))
		$ret = array_map(fn($p) => explode('=', $p, 2)[0], explode(',', $string));
	return $ret;
}

# Build an associative array from a string of type "par1=value1,par2=value2,par3=value3"
function assocPars($string)
{
	$arr_assoc = [];
	if (strlen($string)) {
		$pairs = explode(',', $string);
		foreach ($pairs as $pair) {
			list($key, $value) = explode('=', $pair, 2);
			$arr_assoc[strtolower(trim($key))] = trim($value);
		}
	}
	return $arr_assoc;
}

# Generate an identifier compliant with the otherId field in Dataverse
function otherID ($id)
{
    return str_replace('-','',$id).date("Y");
}

// Sort a Multi-dimensional Array by Value
function aasort (&$array, $key)
{
	$sorter = array();
	$ret = array();
	reset($array);
	$id = abs(intval($key))-1;
	foreach ($array as $ii => $va) {
		$sorter[$ii] = $va[$id];
	}
	asort($sorter);
	foreach ($sorter as $ii => $va) {
		$ret[$ii] = $array[$ii];
	}
	if(intval($key)<0) $array = array_reverse($ret);
	else               $array = $ret;
}

// UFT-8 encode for a string
function utf8_enc ($iso8859_1_string)
{
	if (!is_null($iso8859_1_string))
		return str_replace(array("\r", "\n"), '', mb_convert_encoding($iso8859_1_string, 'UTF-8', 'ISO-8859-1'));
	else
		return iso8859_1_string;
}

// UFT-8 encode for an array
function utf8_converter($array)
{
	array_walk_recursive($array, function(&$item, $key) {
		if (!is_null($item) && !mb_detect_encoding($item, 'utf-8', true)) $item = utf8_enc($item);
	});
	return $array;
}

// PHP function to read TSV file to an array
function tsvToArray($tsv, $header=false)
{
	$tsvArray =array();
	if (($tsvToRead = fopen($tsv, 'r')) !== FALSE) {
		if ($header) $line = fgets($tsvToRead, 1000);
		while (! feof($tsvToRead)) {
			$data = fgetcsv($tsvToRead, 1000, "\t");
			if (gettype($data)==='array') {
				$tsvArray[] = utf8_converter($data);
			}
			if (gettype($data)==='string') {
				$tsvArray[] = utf8_enc($data);
			}
		}
		fclose($tsvToRead);
	}
	return $tsvArray;
}

// Convert TSV to JSON
function csv2json($csv)
{
	$lines = explode("\n", trim($csv));
	$headers = str_getcsv(array_shift($lines));
	$data = [];
	foreach ($lines as $line) {
		$values = str_getcsv($line);
		$row = [];
		foreach ($headers as $i => $header) {
			$row[$header] = $values[$i] ?? '';
		}
		$data[] = $row;
	}
	$json = json_encode($data, JSON_PRETTY_PRINT | JSON_UNESCAPED_UNICODE);
	return $json;
}

// Read a dictionnary
function getDicoValues ($diconame)
{
	global $cvdir;
	$dicovalues = array();
	$dicofile = $cvdir.$diconame.'/'.$diconame.'.txt';
	if ( is_dir($cvdir.$diconame) && file_exists($dicofile) )
		$dicovalues = tsvToArray($dicofile);
	return $dicovalues;
}

// Get the html code for $val
function display_array ($val)
{
	if (is_array($val)) {
		if (count($val) > 1) {
			$val_display = "<ul><li>".implode('</li><li>', $val)."</li></ul>";
		} else {
			$val_display = implode(" ",$val);
			if (empty($val_display)) { $val_display = '-'; }
		}
	} else {
		$val_display ='';
		if (is_string($val))
			$val_display = preg_replace('"^(https?://[^ ]+)"', '<a href="$1" target="_blank">$1</a>',$val);
		if (empty($val_display)) { $val_display = '-'; }
	}
	return $val_display;
}

// Get the label corresponding to the field $field
function get_label($field)
{
	global $checkboite, $txtboite, $listboite, $multiboite, $areaboite, $dateboite;
	if (array_key_exists($field, (array)$checkboite)) $label=$checkboite[$field]['titre'];
	else if (array_key_exists($field, (array)$listboite)) $label=$listboite[$field]['titre'];
	else if (array_key_exists($field, (array)$multiboite)) $label=$multiboite[$field]['titre'];
	else if (array_key_exists($field, (array)$txtboite)) $label=$txtboite[$field]['titre'];
	else if (array_key_exists($field, (array)$areaboite)) $label=$areaboite[$field]['titre'];
	else if (array_key_exists($field, (array)$dateboite)) $label=$dateboite[$field]['titre'];
	else $label='-';
	return $label;
}

// Get the features $arr_names corresponding to the field $field in array $typeboite
function get_features($field, $arr_names, $typeboite)
{
	$arr_features=array();
	foreach( $arr_names as $key ) 
		if (array_key_exists($key, (array)$typeboite[$field])) array_push($arr_features, $key.'='.$typeboite[$field][$key]);
	$features = join(',', $arr_features);
	return $features;
}

// Check if a MongoDB record is accessible
function is_accessible ($record)
{
	global $PRIVATE_ACCESS, $private_auth_file;
	#$GLOB = $_COOKIE; # see inc/access/access.inc
	$GLOB = $_SESSION;
	$path = str_replace('pgd_data', 'mnt', $record['chemin']);
	$file_auth= dirname($path)."/".$private_auth_file;
	$ret=FALSE;
	if ($PRIVATE_ACCESS && file_exists($path) && file_exists($file_auth)) {
		$userkey = file_get_contents($file_auth);
		$ret = ! isset($GLOB['privatekey']) || ($GLOB['privatekey'] !== $userkey);
	}
	return $ret;
}

// Check if a JSON string is valid based on the JSON-Schema
function isJSONValid($json, $schema_file="conf/maggot-schema.json")
{
	global $JSVAPI_URL;
	do {
		if (! is_file($schema_file)) {
			$resp = '{"errors":["the JSON schema file '.$schema_file.' does not exist on the server side"],"valid":false}';
			break;
		}
		$schema = file_get_contents($schema_file);
		$my_json=preg_replace('/\'/', "", $json);
	
		/* Creation of context options */
		$options = array(
			'http' => array(
				'method'  => 'POST',
				'header'  => "Content-Type: application/json",
				'ignore_errors' => true,
				'timeout' =>  10,
				'content' => '{"schema":'.$schema.',"json":'.$my_json.'}',
			),
		);

		/* Creation of the HTTP context */
		$context  = stream_context_create($options);

		/* Execution of the request */
		$resp = file_get_contents($JSVAPI_URL, false, $context);
	} while(0);
	return $resp;
}


//----------------------------
// Find value into Mapping
//----------------------------

function find_mapping($map, $value)
{
	$ret=-1;
	for($i=0; $i<count($map); $i++) {
		$target=$map[$i][0];
		if ($value === $target) { $ret=$i; break; }
	}
	return $ret;
}

//----------------------------
// Find value into Dico
//----------------------------

function find_dico($dico, $value, $arr_concat=array(0,' ',1))
{
	global $accented_chr;
	$ret=-1;
	for($i=0; $i<count($dico); $i++) {
		$target='';
		foreach($arr_concat as $val) {
			if (gettype($val)==='integer') { $target .= $dico[$i][$val]; continue; }
			if (gettype($val)==='string') { $target .= $val; continue; }
		}
		if (is_string($value) && is_string($target) && 
			strtolower(strtr($value, $accented_chr)) === strtolower(strtr($target, $accented_chr))) { $ret=$i; break; }
	}
	return $ret;
}

//----------------------------
// Find value into Vocabulary cache file
//----------------------------

function find_into_cache($name)
{
	global $vocab_cache_file;
	$out = array();
	do {
		if (! file_exists($vocab_cache_file))
			break;
		$dico = tsvToArray($vocab_cache_file);
		$idx=find_dico($dico, $name, array(0));
		if ($idx>-1 && strlen($dico[$idx][1])>0 && strlen($dico[$idx][2])>0)
		$out = array( $dico[$idx][1], $dico[$idx][2] );
		break;
	} while(0);
	
	return $out;
}


//----------------------------
// Get info from Dico / Ontology
//----------------------------
function get_term_info($dico, $name)
{
	global $vocab_cache_file, $nofound_cache_file, $accented_chr;
	$out = array();
	do {
		if (gettype($dico[1])==='string' && file_exists('inc/mapping/'.$dico[1].'.inc'))
		{
			// Search term in the vocabulary cache file
			$out=find_into_cache($name);
			if (count($out)>0)
				break;
		
			// Search term in the 'no found' cache file
			$name_dic = strtolower(strtr($name, $accented_chr));
			if (file_exists($nofound_cache_file)) {
				$arr = explode( "\n", file_get_contents($nofound_cache_file));
				if (in_array($name_dic, $arr))
					break;
			}

			// include the search function for the vocabulary/ontology if based on an Web API
			include_once('inc/mapping/'.$dico[1].'.inc');

			// Search term by API in the thesaurus/ontology web site
			$out = ($dico[1].'_search')($dico, $name);
		
			// if term found, save it in the vocabulary cache file
			if (count($out)>0 && count(find_into_cache($name))==0) {
				file_put_contents($vocab_cache_file, $name_dic."\t".$out[0]."\t".$out[1]."\n", FILE_APPEND | LOCK_EX);
				break;
			}
		
			// if no term found, save it in the 'no found' cache file
			if (count($out)==0 && ! file_exists($nofound_cache_file)) {
				file_put_contents($nofound_cache_file, $name_dic."\n", FILE_APPEND | LOCK_EX);
				break;
			}
			if (count($out)==0 && file_exists($nofound_cache_file)) {
				$arr = explode( "\n", file_get_contents($nofound_cache_file));
				if (! in_array($name_dic, $arr))
					file_put_contents($nofound_cache_file, $name_dic."\n", FILE_APPEND | LOCK_EX);
				break;
			}
			break;
		}

		$idx=find_dico($dico, $name, array(0));
		if ($idx>-1 && strlen($dico[$idx][1])>0 && strlen($dico[$idx][2])>0)
		$out = array( $dico[$idx][1], $dico[$idx][2] );

	} while(0);
	
	return $out;
}

//----------------------------
// Build a box of links
//----------------------------

function box_links($arr_links, $dwnld=0, $help=0)
{
	$html_output = '<fieldset class="border px-3 py-3 rounded">';
	foreach(array_keys($arr_links) as $key) {
		$arr = $arr_links[$key];
		$htmlhelp = $help ? '&nbsp;<a href="'.$arr[5].'" target="doc"><i class="bi-question-octagon-fill" style="color:#1b82c1; font-size: 12px"></i></a>' : '';
		$html_output .= '<div class="row mb-0">';
			$html_output .= '<div class="col-4"><blockquote class="blockquote"><p><i class="bi-caret-right-fill"></i>&nbsp;&nbsp;'.$arr[0].'</p></blockquote></div>';
			$html_output .= '<div class="col-4"><blockquote class="blockquote"><p><i class="bi-eye-fill"></i>&nbsp;&nbsp;<a class="icon-link" href="'.$arr[1].'" target="_blank">'.$arr[2].'</a>'.$htmlhelp.'</p></blockquote></div>';
			if($dwnld)
				$html_output .= '<div class="col-4"><center><button title="Download '.$arr[4].'" class=\"btn btn-success btn-xs bi-download\" style=\"width: 50px;\" onclick="downloadFile(\\\''.$arr[3].'\\\',\\\''.basename($arr[2]).'\\\'); return false;"></button></center></div>';
		$html_output .= '</div>';
	}
	$html_output .= '</fieldset>';
	return $html_output;
}

?>