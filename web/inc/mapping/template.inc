<?php

// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// Replace all '<ws>' by your web service identifier and complete the script where there are 3 dots (...)
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// See ontoportal.inc, ols.inc, skosmos.inc as examples

//----------------------------
// <ws> API
//----------------------------
// Corresponding mapping script when '<ws>' is specified in the CVtype column of the mapping file (conf/mapping.txt)
//
// $map : [array] contains the extracted line as an array coming from the conf/mapping.txt file where the CVtype is equal to <ws>
//        => array(CVname,CVtype,CVurl,CVterms,CVlang)
// $query : [string] CVterm to be mapped to vocabulary/ontology
function <ws>_search($map, $query)
{
	global $LOG;
	$ret=array();
	$server_url=$map[2]; // CVurl
	$ontolist=$map[3];   // CVterms
	$lang=$map[4];       // CVlang

	# Build the curl command
	$cmd='curl -X GET "'.$server_url.'?'. ... .urlencode($query).'"';
	if ($LOG) echo "<script>console.log('".$cmd."');</script>";

	# Execute the curl command
	$output_js=shell_exec($cmd);

	# if output is a json
	$output_arr = json_decode($output_js, true);

	# Test if output is an array, then find the wanted key
	if (gettype($output_arr) == 'array' && array_key_exists('...', $output_arr)) {
		$output_arr = $output_arr['...'];
		if (! empty($output_arr))
			# Get the CVterm URI and the cvterm label => array( CVterm URI, CVterm label )
			$ret = array( $output_arr[0]['...'], $output_arr[0]['...'] ); 
	}

	return $ret;
}

?>