<?php
#
# Bloxberg Blockchain implementation 
#
# Auteur(s): D.JACOB 
# Copyright: (c) INRAE - 2022-2023
#

# --------------------------------------------------------------------------------------------
# Creation of a file expected by the bloxberg API containing 
# i) the URI of the metadata file, 
# ii) meta-metadata (descriptive metadata about the metadata file).
# --------------------------------------------------------------------------------------------
function json2bloxberg($metadata)
{
	global $maggot_fulltitle;
	$APPURL = "https://pmb-bordeaux.fr/maggot";
	$BBjson = '
{
  "@context": {
    "dc": "http://purl.org/dc/terms/"
  },
  "@type": "Dataset",
';
	$dico = getDicoValues('people');
	foreach ($metadata['contacts'] as $contact)
		$BBjson .= '  "dc:creator": "https://orcid.org/'. $dico[find_dico($dico, $contact)][3].'",';
	$BBjson .= '  "dc:language": "en",
  "dc:format": "application/json",
  "dc:title": "'.$metadata[$maggot_fulltitle].'",
  "dc:subject": "https://id.loc.gov/authorities/subjects/sh96000740",
  "dc:date": "'.date('Y-m-d\TH:i:s').substr((string)microtime(), 1, 8).'Z",
  "dc:description": "'.$metadata['description'].'",
  "dc:publisher": [';
	$ror_array = array();
	foreach ($metadata['contacts'] as $contact)
		array_push($ror_array, '"'.get_ROR_url($dico[find_dico($dico, $contact)][2]).'"');
	$BBjson .= join(',',$ror_array).'],
  "dc:rights":"http://spdx.org/licenses/CC-BY-NC-SA-4.0.json",
  "dc:identifier": {
    "@value": "'.$APPURL.'/metadata/'.$metadata['title'].'?format=maggot",
    "@type": "http://purl.org/dc/terms/URI"
    }
}';
	return $BBjson;
}


# --------------------------------------------------------------------------------------------
# Bloxberg API calling  to certify the metadata file on the bloxberg blockchain.
# BC_API_TOKEN : API key provided by bloxberg (Positional parameter 1).
# PUBLICKEY : Bloxberg wallet address (Positional parameter 2).
# --------------------------------------------------------------------------------------------

function exec_cmd_api($cmd)
{
	global $LOG;
	if ($LOG) echo "<script>console.log('".addslashes($cmd)."');</script>";
	$output_js=shell_exec($cmd);
	if ($LOG) echo "<script>console.log('".$output_js."');</script>";
	return $output_js;
}

function get_json_request($BBjson, $metadata_json)
{
	global $PUBLICKEY;
	$jqBBjson = preg_replace('/\s+/', ' ', str_replace("\r", '', str_replace("\n", '', $BBjson)));
	$upload = '{ "publicKey": "'.$PUBLICKEY.'", "crid": [ "'.hash('sha256', $metadata_json).'" ],  "cridType": "sha2-256",  "enableIPFS": false, "metadataJson": "'.addslashes($jqBBjson).'" }';
	return $upload;
}

function certify_api_bloxberg($upload, $mode='POST')
{
	global $BC_API_TOKEN, $BC_SERVER_URL, $LOG;
	$header = 'api_key:'.$BC_API_TOKEN;
	$content_type = "Content-Type: application/json";
	$accept = 'accept: application/json';
	$request = 'createBloxbergCertificate';
	if ($LOG) echo "<script>console.log('".addslashes($upload)."');</script>";
	$cmd='curl -H "'.$header.'" -H "'.$accept.'" -H "'.$content_type.'" -X '.$mode.' '.$BC_SERVER_URL.$request.' -d \''.$upload.'\'';
	return exec_cmd_api($cmd);
}

# --------------------------------------------------------------------------------------------
# CURL request to the bloxberg API (/generatePDF)
# The JSON content that must be sent to the API is the JSON response obtained after certification
# --------------------------------------------------------------------------------------------

function generatePDF_api_bloxberg($BBjson, $output_zip, $mode='POST')
{
	global $BC_API_TOKEN, $BC_SERVER_URL, $PUBLICKEY, $LOG;
	$header = 'api_key:'.$BC_API_TOKEN;
	$content_type = "Content-Type: application/json";
	$accept = 'accept: application/json';
	$request = 'generatePDF';
	$jqBBjson = preg_replace('/\s+/', ' ', str_replace("\r", '', str_replace("\n", '', $BBjson)));
	//$upload = addslashes($jqBBjson);
	$upload = $jqBBjson;
	$cmd='curl -H "'.$header.'" -H "'.$accept.'" -H "'.$content_type.'" -X '.$mode.' '.$BC_SERVER_URL.$request;
	$cmd .= ' -d \''.$upload.'\'';
	$cmd .= ' --output '.$output_zip.' --silent';
	if ($LOG) echo "<script>console.log('".addslashes($cmd)."');</script>";
	return shell_exec($cmd);
}
