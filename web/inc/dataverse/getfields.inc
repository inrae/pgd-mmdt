<?php

// ======================
// Functions generating the elementary structures of the JSON format of Dataverse
// ======================

function simpleField($field, $value)
{
	$value = $value ? str_replace('"', '\"', str_replace("\n", '', $value)) : '';
	return '{"typeName": "'.$field.'","multiple": false,"typeClass": "primitive","value": "'.$value.'"}';
}

function simpleArray($field, $value)
{
	$value = $value ? str_replace("\n", '', $value) : '';
	if (is_array($value)) {
		$str = ''; $sep ='';
		foreach($value as $name) { $str .= $sep.'"'.$name.'"'; $sep=','; }
		$value = $str;
	}
	return '{"typeName": "'.$field.'","multiple": true,"typeClass": "primitive","value":['.$value.']}';
}

function compoundArray($field, $json)
{
	return '{"typeName": "'.$field.'","multiple": true,"typeClass": "compound","value":['.$json.']}';
}

function cvField($field, $value)
{
	return '{"typeName": "'.$field.'","multiple": false,"typeClass": "controlledVocabulary","value": "'.$value.'"}';
}

function cvArray($field, $json)
{
	return '{"typeName": "'.$field.'","multiple": true,"typeClass": "controlledVocabulary","value":['.$json.']}';
}

function cvList($field, $list)
{
	$json='';
	$sep='';
	foreach($list as $name) {
		$json.=$sep.'"'.$name.'"';
		$sep=',';
	}
	return cvArray($field, $json);
}


// ======================
// Functions generating the structures of the JSON format for each of fields within a block
// ======================

include ('inc/dataverse/citation.inc');
include ('inc/dataverse/biomedical.inc');

?>
