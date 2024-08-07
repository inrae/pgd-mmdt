<?php

// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// Replace all '<ws>' by your web service identifier and complete the script where there are 3 dots (...)
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

//----------------------------
// <ws> API
//----------------------------

function <ws>_search($map, $query)
{
	global $LOG;
	$ret=array();
	$server_url=$map[2];
	$ontolist=$map[3];
	$lang=$map[4];

	$cmd='curl -X GET "'.$server_url.'?'. ... .urlencode($query).'"';
	if ($LOG) echo "<script>console.log('".$cmd."');</script>";
	$output_js=shell_exec($cmd);
	$output_arr = json_decode($output_js, true);
	if (gettype($output_arr) == 'array' && array_key_exists('...', $output_arr)) {
		$output_arr = $output_arr['...'];
		if (! empty($output_arr))
			# Get the CVterm URI and the cvterm label 
			$ret = array( $output_arr[0]['...'], $output_arr[0]['...'] ); 
	}
	return $ret;
}

?>