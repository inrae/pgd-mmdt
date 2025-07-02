<?php

// JSON to TSV : Convert the current terminology definition file
function json2tsv()
{
	global $section, $section_search, $required, $shortview, $checkboite, $txtboite, $dateboite, $listboite, $multiboite, $areaboite, $dateboite, $chkbxdico, $listdico;

	$headers = "Field\tSection\tRequired\tSearch\tShortView\tType\tfeatures\tLabel\tPredefined terms\n";
	$content = "";

	// for each section
	foreach( array_keys((array)$section['labels']) as $sec ) {
		// for each field in the section
		foreach( (array)$section[$sec] as $field ) {
			if ($field==='resources') continue;
			$req="N"; $search="N"; $view=''; $type='textbox'; $features=''; $label=''; $terms='';
			if (in_array($field, (array)$required)) $req="Y";
			if (isset($section_search[$sec]) && in_array($field, (array)$section_search[$sec])) $search="Y";
			if (in_array($field, (array)$shortview)) $view=array_search($field, (array)$shortview, TRUE)+1;
			$label = get_label($field);
			if (array_key_exists($field, (array)$checkboite)) {
				$features = get_features($field, array('autocomplete'), $checkboite);
				if (strlen($features)) $features .= ',';
				$type='checkbox';
				if ($checkboite[$field]['open']) { $features .= 'open=1'; } else { $features .= 'open=0'; }
				$terms = join(',', $chkbxdico[$field]);
			}
			else if (array_key_exists($field, (array)$listboite)) {
				$type='dropbox';
				$terms = join(',', $listdico[$field]);
				$features = get_features($field, array('width'), $listboite);
			}
			else if (array_key_exists($field, (array)$multiboite)) {
				$type='multiselect';
				$features = get_features($field, array('autocomplete', 'min', 'width', 'ws', 'onto'), $multiboite);
			}
			else if (array_key_exists($field, (array)$txtboite)) {
				$type='textbox';
				$features = get_features($field, array('autocomplete', 'width'), $txtboite);
			}
			else if (array_key_exists($field, (array)$dateboite)) {
				$type='datebox';
				$features = get_features($field, array('width'), $dateboite);
			}
			else if (array_key_exists($field, (array)$areaboite)) {
				$type='areabox';
				$features = get_features($field, array('rows', 'cols'), $areaboite);
			}
			else $type='unknown';
			$content .= "$field\t$sec\t$req\t$search\t$view\t$type\t$features\t$label\t$terms\n";
		}
	}
	if (array_key_exists('resources', (array)$listdico) && count($listdico['resources'])>0)
		$content .= "resources\tresources\tN\t\t\t\tResources\t\t".implode(',',$listdico['resources'])."\n";
	return $headers.$content;
}

// TSV to JSON : Convert a terminology definition file
function tsv2json($filename, &$outjson)
{
	global $required_fields;

	$msg='';
	do {
		$handle = fopen($filename, "r");
		if (! $handle) break;

		$section=[]; $section['labels']=[];
		$section_search=[]; $section_search['labels']=[];
		$required=[];
		$shortview=[]; $orderview=[];
		$txtboite=[];
		$listboite=[];
		$checkboite=[];
		$multiboite=[];
		$areaboite=[];
		$dateboite=[];
		$checkdico=[];
		$listdico=[];

		// Header
		if (! fgets($handle)) break;

		// Content
		$array_fields = array();
		while (($line = fgets($handle)) !== false) {
			$val = explode("\t", $line);
			array_push($array_fields, $val[0]);
			$features = explode(',', str_replace(', ', ',', $val[6]));
			// section
			if ( ! array_key_exists($val[1], $section['labels']) ) {
				$section['labels'][$val[1]] = strtoupper($val[1]);
				$section[$val[1]] = [];
			}
			if ( ! array_key_exists($val[0], $section[$val[1]]) )
				array_push( $section[$val[1]], $val[0] );
			// section_search
			if ( $val[3]=='Y' && ! array_key_exists($val[1], $section_search['labels']) ) {
				$section_search['labels'][$val[1]] = strtoupper($val[1]);
				$section_search[$val[1]] = [];
			}
			if ( $val[3]=='Y' && ! array_key_exists($val[0], $section_search[$val[1]]) )
				array_push( $section_search[$val[1]], $val[0] );
			// required
			if ($val[2]=='Y' && ! in_array($val[0], $required) ) {
				array_push( $required, $val[0] );
			}
			// shortview
			if (is_numeric($val[4]) && ! in_array($val[0], $shortview) ) {
				array_push( $orderview, intval($val[4]) );
				array_push( $shortview, $val[0] );
			}
			//checkboite
			if ( $val[5]=='checkbox' && ! array_key_exists($val[0], $checkboite) ) {
				$checkboite[$val[0]]=[];
				$checkboite[$val[0]]['titre']=utf8_enc($val[7]);
				foreach ( $features as $value )
					if (strlen($value)) {
						$arr = explode('=',$value);
						if ($arr[1]=='0' ) { $arr[1]=FALSE; }
						if ($arr[1]=='1' ) { $arr[1]=TRUE; }
						$checkboite[$val[0]][$arr[0]]=$arr[1];
					}
			}
			//listboite
			if ( $val[5]=='dropbox' && ! array_key_exists($val[0], $listboite) ) {
				$listboite[$val[0]]=[];
				$listboite[$val[0]]['titre']=utf8_enc($val[7]);
				foreach ( $features as $value )
					if (strlen($value)) {
						$arr = explode('=',$value);
						$listboite[$val[0]][$arr[0]]=$arr[1];
					}
			}
			//multiboite
			if ( $val[5]=='multiselect' && ! array_key_exists($val[0], $multiboite) ) {
				$multiboite[$val[0]]=[];
				$multiboite[$val[0]]['titre']=utf8_enc($val[7]);
				foreach ( $features as $value )
					if (strlen($value)) {
						$arr = explode('=',$value);
						if ($arr[0]=='open' && $arr[1]=='0' ) { $arr[1]=FALSE; }
						if ($arr[0]=='open' && $arr[1]=='1' ) { $arr[1]=TRUE; }
						$multiboite[$val[0]][$arr[0]]=$arr[1];
					}
			}
			//txtboite
			if ( $val[5]=='textbox' && ! array_key_exists($val[0], $txtboite) ) {
				$txtboite[$val[0]]=[];
				$txtboite[$val[0]]['titre']=utf8_enc($val[7]);
				foreach ( $features as $value )
					if (strlen($value)) {
						$arr = explode('=',$value);
						$txtboite[$val[0]][$arr[0]]=$arr[1];
					}
			}
			//dateboite
			if ( $val[5]=='datebox' && ! array_key_exists($val[0], $dateboite) ) {
				$dateboite[$val[0]]=[];
				$dateboite[$val[0]]['titre']=utf8_enc($val[7]);
				foreach ( $features as $value )
					if (strlen($value)) {
						$arr = explode('=',$value);
						$dateboite[$val[0]][$arr[0]]=$arr[1];
					}
			}
			//areaboite
			if ( $val[5]=='areabox' && ! array_key_exists($val[0], $areaboite) ) {
				$areaboite[$val[0]]=[];
				$areaboite[$val[0]]['titre']=utf8_enc($val[7]);
				foreach ( $features as $value )
					if (strlen($value)) {
						$arr = explode('=',$value);
						$areaboite[$val[0]][$arr[0]]=$arr[1];
					}
			}
			//checkdico
			if ( $val[5]=='checkbox' && ! array_key_exists($val[0], $checkdico) ) {
				$checkdico[$val[0]]= [];
				$str_dico = str_replace('---', '-', str_replace(array("\r", "\n"), '', preg_replace('/, ?/','@',utf8_enc($val[8]))));
				foreach ( explode('@',$str_dico) as $value ) array_push( $checkdico[$val[0]], $value );
			}
			//listdico && resources
			if ( ($val[5]=='dropbox' || $val[0]=='resources') && ! array_key_exists($val[0], $listdico) ) {
				$listdico[$val[0]]=[];
				$str_dico = str_replace('---', '-', str_replace(array("\r", "\n"), '', preg_replace('/, ?/','@',utf8_enc($val[8]))));
				foreach ( explode('@',$str_dico) as $value ) array_push( $listdico[$val[0]], $value );
			}
		}
		fclose($handle);

		// resources
		if ( ! array_key_exists('resources', $section) ) {
			$section['labels']['resources'] = 'RESOURCES';
			$section['resources']=array('resources');
			$listdico['resources']=[];
		}

		foreach( $required_fields as $field )
			if (! in_array($field, $array_fields)) $msg .= 'ERROR: the field "'.$field.'" is required !! <br>';

		if (strlen($msg))
			break;

		array_multisort($orderview, SORT_NUMERIC, SORT_ASC, $shortview);

		$config_terms=array( 'sections'=>$section, 'sections_search'=>$section_search, 'required'=>$required, 'shortview'=>$shortview,
							'checkboite'=>$checkboite, 'listboite'=>$listboite, 'multiboite'=>$multiboite, 'txtboite'=>$txtboite, 'areaboite'=>$areaboite, 'dateboite'=>$dateboite,
							'chkbxdico'=>$checkdico, 'listdico'=>$listdico );

		$outjson = json_encode($config_terms, JSON_UNESCAPED_UNICODE);
		if (strlen($outjson)==0) {
			$msg = 'ERROR: Encoding to JSON format failed.';
		}

	}  while(0);
	return $msg;
}

// TSV to JSON-Schema : Convert a terminology definition file
function tsv2schema($filename, $url_schema)
{
	global $required;

	$outschema ='{"$schema": "http://json-schema.org/draft-07/schema#", "$id": "'.$url_schema.'",';
	$outschema.='"title": "maggot-package","description": "Description of the maggot JSON schema", "type": "object"';

	$profile = '"profile": { "title": "maggot-package", "description": "JSON schema for the maggot tool", "type": "string" }';
	$def_array = '"def_array": { "type": "array", "minItems": 0, "uniqueItems": true, "items": { "type": "string" } }';
	$def_resource = '"resource": {"title":"Resource","type": "object","properties": {"datatype": {"title": "Data type","type": "string"},"description": {"title": "Data description","type": "string"},"location": {"title": "Data location","type": "string"}}}';
	$def_resources = '"resources": {"type": "array","minItems": 0,"items": {"$ref": "#/definitions/resource"}}';

	$def_required = '"required": [ "profile"';
	foreach ( $required as $value ) $def_required .= ',"'.$value.'"';
	$def_required .= ' ]';

	$definitions = [];
	$properties = [];

	array_push($definitions, $def_array, $def_resource, $def_resources);
	array_push($properties, $profile);

	do {

		$handle = fopen($filename, "r");
		if (! $handle) break;

		// Header
		if (! fgets($handle)) break;

		// Content
		while (($line = fgets($handle)) !== false) {
			$val = explode("\t", $line);
			// resources
			if ( $val[0]=='resources' ) continue;
			// Process some columns : features, label, predefined terms
			$arr = assocPars($val[6]);
			$label = str_replace(array("\r", "\n"), '', $val[7]);
			if (in_array($val[5], array('checkbox','dropbox'))) {
				$arr_terms = explode(',', str_replace('---', '-', str_replace(', ', ',', str_replace(array("\r", "\n"), '', utf8_enc($val[8])))));
				#$arr_terms = array_filter($arr_terms, function ($value) { return !preg_match('/^-/',$value); });
			}
			// checkbox
			if ( $val[5]=='checkbox' && $arr['open']==1) {
				array_push($properties, '"'.$val[0].'": { "title" : "'.$label.'", "$ref": "#/definitions/def_array" }');
			} elseif ( $val[5]=='checkbox' && $arr['open']==0) {
				$min = in_array($val[0], $required) ? 1 : 0;
				array_push($definitions, '"'.$val[0].'_items": { "type": "string", "enum": ["'.implode('","',$arr_terms).'"]}');
				array_push($definitions,'"'.$val[0].'_array": { "type": "array", "minItems": '.$min.', "items": { "$ref": "#/definitions/'.$val[0].'_items" } }');
				array_push($properties, '"'.$val[0].'": { "title" : "'.$label.'", "$ref": "#/definitions/'.$val[0].'_array" }');
			// dropbox
			} elseif ( $val[5]=='dropbox') {
				array_push($definitions, '"'.$val[0].'_items": { "type": "string", "enum": ["'.implode('","',$arr_terms).'"]}');
				array_push($properties, '"'.$val[0].'": { "title" : "'.$label.'", "$ref": "#/definitions/'.$val[0].'_items" }');
			// multiselect
			} elseif ( $val[5]=='multiselect' ) {
				array_push($properties, '"'.$val[0].'": { "title" : "'.$label.'", "$ref": "#/definitions/def_array" }');
			} else {
			// textbox, areabox
				array_push($properties, '"'.$val[0].'": { "title" : "'.$label.'", "type": "string" }');
			}
		}
		// resources
		array_push($properties, '"resources": { "title" : "Resources", "$ref": "#/definitions/resources" }');
		fclose($handle);
		$outschema .= ', "definitions": {'.join(",", $definitions).'}, "properties": {'.join(",", $properties).'},'.$def_required;

	}  while(0);

	$outschema .='}';

	return $outschema;
}

// TSV to Template Doc  : Convert a terminology definition file to a template file for terminology documentation
function tsv2doctmpl($filename)
{
	global $RESMEDIA;
	$doctmpl="Type\tName\tDescription\n";
	$section='';
	do {
		$handle = fopen($filename, "r");
		if (! $handle) break;

		// Header
		if (! fgets($handle)) break;

		// Content
		while (($line = fgets($handle)) !== false) {
			$val = explode("\t", $line);
			if ($val[1] == 'resources') {
				$section = $val[1];
				continue;
			}
			if ( $val[1] != $section ) {
				$section = $val[1];
				$doctmpl .= "section\t$section\tThis section allows you to define ...\n";
			}
			$desc='';
			if ($val[0] == 'title')
				$desc = 'Give your dataset a short name. This name will then be used to identify it and reference the metadata page via a URL. Do not use spaces or non-alphanumeric characters. Only letters, numbers and underscores are allowed.';
			$doctmpl .= "field\t".$val[0]."\t$desc\n";
			if ($val[5] =='dropbox' ) {
				$opts = explode(",", $val[8]);
				if ($opts[0]=='-') {
					array_shift($opts);
					$doctmpl .= "option\t".str_replace(array("\r\n", "\n", "\r"), '', implode(',',$opts))."\t\n";
				} else {
					foreach( $opts as $text)
						$doctmpl .= "option\t".preg_replace('~[[:cntrl:]]~', '',$text)."\t\n";
				}
			}
		}
		$doctmpl .= "section\tresources\tThis section allows you to define all the resources you want\n";
		$doctmpl .= "field\ttype\tChoose the type of the resoource\n";
		if ($RESMEDIA) $doctmpl .= "field\tmedia\tChoose the media type of the resoource\n";
		$doctmpl .= "field\tdescription\tProvide a concise and accurate description of the resource\n";
		$doctmpl .= "field\tlocation\tA location can be anything:  a URL link, an absolute path in a tree, a text, ...\n";
		fclose($handle);

	}  while(0);
	return $doctmpl;
}

// TSV to MD : Convert a terminology documentation file to Markdown
function tsv2md($filename)
{
	global $checkboite, $RESMEDIA;

	$md = "# Metadata documentation\n----\n\n";
	$md .= "<font size=\"+1\"><b>Set of metadata as defined in the terminology.</b></font>\n\n---\n\n";
	$md .= ".\n\n";
	$section='';

	do {
		$handle = fopen($filename, "r");
		if (! $handle) { echo "File cannot be opened"; break; }

		// Header
		if (! fgets($handle)) break;

		// Content
		while (($line = fgets($handle)) !== false) {
			$val = explode("\t", $line);
			// section
			if ($val[0] == 'section') {
				if ( strlen($section)>0 ) $md .= "</div>\n\n.\n";
				$md .= "## ".strtoupper($val[1])."\n\n* ".$val[2]."\n\n<div class=\"decal\" markdown=\"1\">\n";
				$section=$val[1];
				continue;
			}
			// field
			if ($val[0] == 'field') {
				if ($section != 'resources') {
					$md .= "### ".get_label($val[1])."\n\n* ".$val[2]."\n\n";
					if (array_key_exists($val[1], (array)$checkboite) && $checkboite[$val[1]]['open'])
						$md .= "* If an item does not appear in the choices proposed, it is possible to enter it manually in the field provided for this purpose. Create a line for each new item to be added.\n\n";
				} else {
					if ($val[1]=='type') $md .= "### Resource Type\n\n* ".$val[2]."\n\n";
					if ($RESMEDIA && $val[1]=='media') $md .= "### Media Type\n\n* ".$val[2]."\n\n";
					if ($val[1]=='description') $md .= "### Description\n\n* ".$val[2]."\n\n";
					if ($val[1]=='location') $md .= "### Location\n\n* ".$val[2]."\n\n";
				}
				continue;
			}
			// option
			if ($val[0] == 'option') {
				$md .= "    *  **".$val[1]."** :  ".$val[2]."\n";
				continue;
			}
		}
		$md .= "\n<br>";

	} while(0);

	$md .= "\n\n<br><br>";
	return $md;
}

// TSV to HTML : Convert a terminology definition file
function tsv2html($filename)
{
	$html='';
	do {
		$handle = fopen($filename, "r");
		if (! $handle) { echo "File cannot be opened"; break; }

		$html .= <<<EOF
<meta http-equiv="Content-Type" content="text/html;charset=utf-8" />
<link rel="icon" href="../favicon.ico" />
<style>
table { padding: 0; font-family: Tahoma; font-size: 9pt; border: 0px solid #B0BDCC; border-spacing: 3px; min-width: 800px;}
.head { line-height: 20px; background-color: #4a86e8; color: white; }
.tr1 { line-height: 14px; }
.tr2 { line-height: 20px; background-color:#EFF4F9; }
</style>
EOF;

	// Header
	if (($line = fgets($handle)) === false) break;
	$html .= '<table><tr class="head">'; // open table
	// render headers
	foreach( explode("\t", $line) as $text) {
		if (trim($text)=='Label') { $html .= '<th style="min-width: 150px; width: 250px;">&nbsp;'.$text.'&nbsp;</th>'; continue; }
		if (trim($text)=='Predefined terms') { $html .= '<th style="min-width: 500px;">&nbsp;'.$text.'&nbsp;</th>'; continue; }
		$html .= '<th>&nbsp;'.$text.'&nbsp;</th>';
	}

	// Content
	$cnt=0;
	while (($line = fgets($handle)) !== false) {
		$val = explode("\t", $line);
		if (preg_match('/^#/',$val[0])) {
			$html .= '</table><table><tr class="head">';
			foreach( explode("\t", $line) as $text)
				$html .= '<th>&nbsp;'.$text.'&nbsp;</th>';
			continue;
		}
		if (($cnt%2)==0) { $TR='<tr class="tr1">'; } else { $TR='<tr class="tr2">'; }
		$html .= $TR.'<td>'.join('&nbsp;</td><td>',preg_replace('/([,:])(?! )/','$1 $2',$val)).'&nbsp;</td></tr>';
		$cnt++;
	}
	$html .= '</table>'; // open table

	}  while(0);
	return $html;
}

// Transfers all descriptions from a config_doc file to the new one
function mergeDoc2tmpl($docFile, $docNewFile)
{
	// Source
	$doc = tsvToArray($docFile, $header=true);
	// Target
	$docNew="Type\tName\tDescription\n";
	do {
		$handle = fopen($docNewFile, "r");
		if (! $handle) break;

		// Header
		if (! fgets($handle)) break;

		// Content
		while (($line = fgets($handle)) !== false) {
			$val = explode("\t", $line);
			if (count($val)<3) continue;
			// target description
			$desc = str_replace (array("\r\n", "\n", "\r"), ' ', $val[2]);
			// Gets the source description of the same entry
			foreach($doc as $arr)
				if (count($arr)==3 && $arr[0]==$val[0] && $arr[1]==$val[1]) { $desc=$arr[2]; break; }
			$docNew .= $val[0]."\t".$val[1]."\t$desc\n";
		}
		fclose($handle);
	}  while(0);
	return $docNew;
}

?>