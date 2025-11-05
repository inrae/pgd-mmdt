<?php

//----------------------------
// BioPortal API
//----------------------------
// Corresponding mapping script when 'bioportal' is specified in the CVtype column of the mapping file (conf/mapping.txt)
//
function bioportal_search($map, $query)
{
	$ret=array();
	$server_url = $map[2];
	$ontology=$map[3];
	$apikey = 'f709257f-06a4-450c-a21f-89727cc7fbe1';
	$LOG = 0;

	$server_url = "https://data.bioontology.org";
	$base_url = $server_url.'/search';
	$params = [ "q" => $query, "format" => "json", "ontologies" => "" ];
	$url = $base_url . "?" . http_build_query($params);
	if (strlen($ontology)>0 && $ontology !== 'all') $url .= $ontology;
	if ($LOG) echo "<script>console.log('".$url."');</script>";
	
	$ch = curl_init($url);
	curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
	curl_setopt($ch, CURLOPT_FOLLOWLOCATION, true);
	curl_setopt($ch, CURLOPT_HTTPHEADER, [
		"Authorization: apikey token=$apikey",
	]);
	$response = curl_exec($ch);
	$httpCode = curl_getinfo($ch, CURLINFO_HTTP_CODE);
	$contentType = curl_getinfo($ch, CURLINFO_CONTENT_TYPE);
	if ($LOG) echo "<script>console.log('".$httpCode.' - '.$contentType."');</script>";

	if (strpos($contentType, "application/json") === false || $httpCode !== 200 || curl_errno($ch)) {
		curl_close($ch);
		return $ret;
	}
	curl_close($ch);

	$json = json_decode($response, true);
	if (json_last_error() !== JSON_ERROR_NONE) return $ret;

	$col = $json['collection'];
	$id = $col[0]['@id'];
	$onto = basename($col[0]['links']['ontology']);
	$ret = array( $onto, $id );
	if ($LOG) echo "<script>console.log('".$id.' - '.$onto."');</script>";

	return $ret;
}

?>
