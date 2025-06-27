<?php

// === Proxy serving as an ORCID API call ===

// Retrieve a Token
function get_token_orcid() {
	global $ORCID_OAUTH_URL, $ORCID_CLIENT_ID, $ORCID_CLIENT_SECRET, $ORCID_MAX_TIME;
	$cmd = 'curl -s -H "Accept: application/json" --max-time '.$ORCID_MAX_TIME.' --data "client_id='.$ORCID_CLIENT_ID.'&client_secret='.$ORCID_CLIENT_SECRET.'&grant_type=client_credentials&scope=/read-public" '.$ORCID_OAUTH_URL.'/token';
	$output_js=shell_exec($cmd);
	$output_arr = json_decode($output_js, true);
	return $output_arr;
}

// Send an API query
function get_orcid_query($token, $fn, $gn=null) {
	global $ORCID_API_URL, $ORCID_MAX_TIME;
	if (is_null($gn))
		$query  = 'family-name:'.$fn;
	else
		$query  = 'given-names:'.$gn.'+AND+family-name:'.$fn;
	$query .= '&fl=orcid,given-names,family-name,email,current-institution-affiliation-name';
	# Submit query
	$cmd = 'curl -s -H "Accept: text/csv" --max-time '.$ORCID_MAX_TIME.' -H "Authorization: Bearer '.$token.'" '.$ORCID_API_URL.'/?q='.$query;
	return 	csv2json(shell_exec($cmd));
}

// ORCID API call
if (isset($_GET['orcid']))
{
	// Defaut response
	$def = '"oricid":"","given-names":"Not found","family-name": "", "email":""';
	$inst1 = '"current-institution-affiliation-name": "maybe the name is misspelled"';
	$inst2 = '"current-institution-affiliation-name": "Wrong Client id/secret"';
	$json = '[{'.$def.','.$inst1.'}]';
	do {
		// Get Token
		$output_arr = get_token_orcid();
		if (! isset($output_arr['access_token'])) {
			$json = '[{'.$def.','.$inst2.'}]';
			break;
		}
		$token = $output_arr['access_token'];

		// Query
		$query=trim($_GET['orcid']);

		// First query : given-names+family-name
		$json_resp = '[]';
		if (is_string($query) && preg_match('/,/',$query)) {
			$req = explode(",", $query);
			$json_resp = get_orcid_query($token, $req[0],$req[1]);
			$query = $req[0];
		}

		// Second query : family-name only
		if ($json_resp == '[]')
			$json_resp = get_orcid_query($token, $query);

		if ($json_resp !== '[]')
			$json = $json_resp;

	} while(0);

	// Send response
	header('Content-Type: application/json');
	echo $json;
	exit(0);
}

?>
