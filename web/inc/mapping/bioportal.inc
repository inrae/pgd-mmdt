<?php

//----------------------------
// BioPortal API
//----------------------------
// Corresponding mapping script when 'bioportal' is specified in the CVtype column of the mapping file (conf/mapping.txt)
//
// Note (oct 2025) : We are using the API with a token because simple searches without a token no longer work correctly 
//                   via curl calls from a server (server side curl blocked by Cloudflare, which uses anti-scraping methods)
//                   See https://scrapfly.io/blog/posts/how-to-bypass-cloudflare-anti-scraping

function bioportal_search($map, $query)
{
	$ret=array();
	$server_url = $map[2];
	$ontology=$map[3];
	if (!file_exists('conf/bpcode.txt')) return $ret;
	$headercode = explode("\n", file_get_contents('conf/bpcode.txt'))[0];
	$LOG = 0;

	$server_url = "https://data.bioontology.org";
	$base_url = $server_url.'/search';
	$params = [ "q" => $query, "format" => "json", "ontologies" => "" ];
	$url = $base_url . "?" . http_build_query($params);
	if (strlen($ontology)>0 && $ontology !== 'all') $url .= $ontology;
	if ($LOG) echo "<pre>".$url."</pre>";
	if ($LOG) echo "<pre>".$headercode."</pre>";

	$ch = curl_init($url);
	curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
	curl_setopt($ch, CURLOPT_FOLLOWLOCATION, true);
	curl_setopt($ch, CURLOPT_HTTPHEADER, [ 'Authorization: apikey token='.base64_decode($headercode) ]);

	$response = curl_exec($ch);
	$httpCode = curl_getinfo($ch, CURLINFO_HTTP_CODE);
	$contentType = curl_getinfo($ch, CURLINFO_CONTENT_TYPE);
	if ($LOG) echo "<pre>".$httpCode.' - '.$contentType."</pre>";

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
	if ($LOG) echo "<pre>".$onto.' - '.$id."</pre>";

	$ret = array( $onto, $id );
	return $ret;
}


?>
