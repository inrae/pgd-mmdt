<?php

//----------------------------
// Skosmos API
//----------------------------

function skosmos_search($map, $query)
{
	global $LOG;
	$ret=array();
	$server_url=$map[2];
	$vocab=$map[3];
	$lang=$map[4];
	$cmd='curl -X GET "'.$server_url.'/rest/v1/search?type=skos%3AConcept&offset=0&vocab='.$vocab.'&lang='.$lang.'&query='.urlencode($query).'"';
	if ($LOG) echo "<script>console.log('".$cmd."');</script>";
	$output_js=shell_exec($cmd);
	$output_arr = json_decode($output_js, true);
	if (gettype($output_arr) == 'array' && array_key_exists('results', $output_arr)) {
		$output_arr = $output_arr['results'];
		if (! empty($output_arr))
			$ret = array( $output_arr[0]['vocab'], $output_arr[0]['uri'] ); 
	}
	return $ret;
}

?>
