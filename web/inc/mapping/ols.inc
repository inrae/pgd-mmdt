<?php

//----------------------------
// OLS API
//----------------------------
// Corresponding mapping script when 'ols' is specified in the CVtype column of the mapping file (conf/mapping.txt)
//
function ols_search($map, $query)
{
	global $LOG;
	$ret=array();
	$server_url=$map[2];
	$ontolist=$map[3];
	$lang=$map[4];
	$ontology='';
	if (strlen($ontolist)>0) {
		$arr_onto=explode(",",$ontolist);
		foreach($arr_onto as $onto) $ontology .= 'ontology='.strtolower($onto).'&';
	}
	$cmd='curl -X GET "'.$server_url.'/api/search?'.$ontology.'exact=true&obsoletes=false&local=false&rows=100&format=json&lang='.$lang.'&q='.urlencode($query).'"';
	if ($LOG) echo "<script>console.log('".$cmd."');</script>";
	$output_js=shell_exec($cmd);
	$output_arr = json_decode($output_js, true);
	if (gettype($output_arr) == 'array' && array_key_exists('response', $output_arr)) {
		$output_arr = $output_arr['response']['docs'];
		if (! empty($output_arr))
			$ret = array( $output_arr[0]['ontology_prefix'], $output_arr[0]['iri'] ); 
	}
	return $ret;
}

?>