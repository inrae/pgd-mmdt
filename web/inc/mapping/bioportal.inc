<?php

//----------------------------
// BioPortal API
//----------------------------

function bioportal_search($map, $query)
{
	global $LOG;
	$ret=array();
	$server_url=$map[2];
	$ontology=$map[3];
	$request = $server_url.'/search/json_search/?q='.urlencode($query);
	$request .= '&target_property=name&ontologies=';
	if (strlen($ontology)>0 && $ontology !== 'all') $request .= $ontology;
	$cmd='curl -X GET "'.$request.'"';
	if ($LOG) echo "<script>console.log('".$cmd."');</script>";
	$output_str=shell_exec($cmd);
	if (gettype($output_str) == 'string' && strlen($output_str)) {
		$arr = explode('|', explode('~!~', $output_str)[0]);
		$ret = array( $arr[3], $arr[1] );
	}
	return $ret;
}

?>
