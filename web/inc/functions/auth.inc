<?php

/**
 * Get header Authorization
 * */
function getAuthorizationHeader(){
    $headers = null;
    if (isset($_SERVER['Authorization'])) {
        $headers = trim($_SERVER["Authorization"]);
    } else if (isset($_SERVER['HTTP_AUTHORIZATION'])) { //Nginx or fast CGI
        $headers = trim($_SERVER["HTTP_AUTHORIZATION"]);
    } elseif (function_exists('apache_request_headers')) {
        $requestHeaders = apache_request_headers();
        // Server-side fix for bug in old Android versions (a nice side-effect of this fix means we don't care about capitalization for Authorization)
        $requestHeaders = array_combine(array_map('ucwords', array_keys($requestHeaders)), array_values($requestHeaders));
        //print_r($requestHeaders);
        if (isset($requestHeaders['Authorization'])) {
            $headers = trim($requestHeaders['Authorization']);
        }
    }
    return $headers;
}

/**
 * get access token from header
 * */
function getBearerToken() {
    global $DEV;
    $headers = getAuthorizationHeader();
    // HEADER: Get the access token from the header
    if (!empty($headers)) {
        if (preg_match('/Bearer\s((.*)\.(.*)\.(.*))/', $headers, $matches)) {
            $payload = $matches[3];
            $jsondata = base64_decode($payload);
            if ($DEV) print "<script>console.log(".$jsondata.")</script>";
            $data = json_decode($jsondata, true);
            return $data;
        }
    }
    return null;
}

/**
 * populate some global variables relative to access rigths
   $AUTH_TOKEN : payload from the JWT as an array
   $AUTH_USER : login name
   $AUTH_ROLES : array of roles 
   $USER : login name depending on how the user logs in. If it belongs to the admin group, then populate as admin.
 * */
$AUTH_TOKEN=getBearerToken();
if (! is_null($AUTH_TOKEN)) {
   $AUTH_USER = $AUTH_TOKEN['preferred_username'];
   $AUTH_ROLES = array();
   if (array_key_exists('resource_access', $AUTH_TOKEN) &&
       array_key_exists($AUTH_TOKEN['azp'], $AUTH_TOKEN['resource_access']) && 
       array_key_exists('roles', $AUTH_TOKEN['resource_access'][$AUTH_TOKEN['azp']]))
       $AUTH_ROLES = $AUTH_TOKEN['resource_access'][$AUTH_TOKEN['azp']]['roles'];
       $USER = in_array($ADMINGROUP, $AUTH_ROLES) ? $ADMINGROUP : $AUTH_USER;
} else {
   $USER = isset($_SERVER['PHP_AUTH_USER']) ? $_SERVER['PHP_AUTH_USER'] : '';
}

?>
