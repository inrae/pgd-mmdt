<?php
// OntoPortal API call
if (isset($_GET['onto']))
{
	$portal = $_GET['onto'];

	switch($portal) {
		case 'bioportal';
			$API_URL = 'https://data.bioontology.org/search';
			break;
		case 'agroportal';
			$API_URL = 'https://data.agroportal.eu/search';
			break;
		default;
			$API_URL = '';
			break;
	}

	function fail(int $code): never {
		http_response_code($code);
		echo '[]';
		exit;
	}

	header('Content-Type: application/json; charset=utf-8');


	// --- Input validation ---

	if (strlen($API_URL)==0) {
		echo json_encode([], JSON_UNESCAPED_UNICODE);
		exit(0);
	}

	$q = trim($_GET['q'] ?? '');
	if (mb_strlen($q) < 3 || mb_strlen($q) > 100) fail(400);

	$ontologies = $_GET['ontology'] ?? '';
	if ($ontologies !== '' && !preg_match('/^[A-Za-z0-9_,-]+$/', $ontologies)) fail(400);

	$API_KEY = get_apikey($portal);
	$LIMIT   = $_GET['limit'] ?? 99;


	// --- API call ---

	$params = [
		'q'        => $q,
		'suggest'  => 'true',
		'include'  => 'prefLabel',
		'pagesize' => $LIMIT,
		'apikey'   => $API_KEY,
	];
	if ($ontologies !== '') $params['ontologies'] = $ontologies;

	$ch = curl_init($API_URL . '?' . http_build_query($params));
	curl_setopt_array($ch, [
		CURLOPT_RETURNTRANSFER => true,
		CURLOPT_TIMEOUT        => 8,
		CURLOPT_HTTPHEADER     => ['Accept: application/json'],
	]);
	$body = curl_exec($ch);
	$code = curl_getinfo($ch, CURLINFO_HTTP_CODE);
	curl_close($ch);
	
	if ($body === false || $code !== 200) fail(502);

	if ($_GET['origapi']) {
		echo $body;
		exit(0);
	}

	$json = json_decode($body, true);
	$out  = [];
	foreach (($json['collection'] ?? []) as $el) {
		$out[] = [
			'label'    => $el['prefLabel'] ?? '',
			'ontology' => basename($el['links']['ontology'] ?? ''),
		];
	}

	// Short browser cache: limits repeated requests
	header('Cache-Control: public, max-age=300');
	echo trim(json_encode($out, JSON_UNESCAPED_UNICODE));
	exit(0);
}
