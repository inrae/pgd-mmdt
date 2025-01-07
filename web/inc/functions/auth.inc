<?php

/*
 * Get key value in header
 */
function getHeader($key)
{
    $headers = null;
    if (isset($_SERVER[$key])) {
        $headers = trim($_SERVER[$key]);
    } else if (isset($_SERVER['HTTP_'.strtoupper($key)])) { //Nginx or fast CGI
        $headers = trim($_SERVER['HTTP_'.strtoupper($key)]);
    } elseif (function_exists('apache_request_headers')) {
        $requestHeaders = apache_request_headers();
        $requestHeaders = array_combine(array_map('ucwords', array_keys($requestHeaders)), array_values($requestHeaders));
        if (isset($requestHeaders[$key])) {
            $headers = trim($requestHeaders[$key]);
        }
    }
    return $headers;
}

/*
 * get access token from header
 */
function getBearerToken()
{
    global $DEV;
    $headers = getHeader('Authorization');
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

// DEV mode only for the home page if request
//if (isset($_GET['dev']) && strlen($_SERVER['QUERY_STRING']) == 3) $DEV=1;

/*
 *  populate some global variables relative to access rigths
 *  $AUTH_TOKEN : payload from the JWT as an array
 *  $AUTH_USER : login name
 *  $AUTH_ROLES : array of roles 
 *  $USER : login name depending on how the user logs in. If it belongs to the admin group, then populate as admin.
 */
do {

   $AUTH_ROLES = array();
   $USER = '';

   $AUTH_TOKEN=getBearerToken();
   if (is_null($AUTH_TOKEN)) {
       $USER = isset($_SERVER['PHP_AUTH_USER']) ? $_SERVER['PHP_AUTH_USER'] : '';
       break;
   }

   $AUTH_IP_NAME = getHeader('IP_NAME'); 
   if (empty($AUTH_IP_NAME)) {
       break;
   }

   $AUTH_ID_TOKEN = getHeader('ID_Token');
   if ($DEV) print "<script>console.log('".$AUTH_IP_NAME." : ".$AUTH_ID_TOKEN."')</script>";

   if ($AUTH_IP_NAME == "KEYCLOAK") {
      $AUTH_USER = $AUTH_TOKEN['preferred_username'];
      if (array_key_exists('resource_access', $AUTH_TOKEN) &&
         array_key_exists($AUTH_TOKEN['azp'], $AUTH_TOKEN['resource_access']) && 
         array_key_exists('roles', $AUTH_TOKEN['resource_access'][$AUTH_TOKEN['azp']]))
         $AUTH_ROLES = $AUTH_TOKEN['resource_access'][$AUTH_TOKEN['azp']]['roles'];
      $USER = in_array($ADMINGROUP, $AUTH_ROLES) || in_array($AUTH_USER, $ADMINUSERS) ? $ADMINGROUP : $AUTH_USER;
      break;
   }

   if (! is_null($AUTH_IP) && isset($AUTH_IP[$AUTH_IP_NAME])) {
      $INFOS = $AUTH_IP[$AUTH_IP_NAME];
      $USERINFO_ENDPOINT =  $INFOS[0];
      $cmd = "curl -X GET -H \"Authorization: Bearer ".$AUTH_ID_TOKEN."\" $USERINFO_ENDPOINT";
      if ($DEV) echo "<script>console.log('".addslashes($cmd)."');</script>";
      $output_js=shell_exec($cmd);
      if ($DEV) echo "<script>console.log(".$output_js.");</script>";
      $output_arr = json_decode($output_js, true);
      $AUTH_TOKEN['name'] = $output_arr['given_name'].' '.$output_arr['family_name'];
      $AUTH_TOKEN = array_merge($AUTH_TOKEN, $output_arr);
      if ($DEV) echo "<script>console.log(".json_encode($AUTH_TOKEN).");</script>";
      $AUTH_USER = $AUTH_TOKEN['preferred_username'];
      if (isset($AUTH_TOKEN[$INFOS[1]]))
          if (is_array($AUTH_TOKEN[$INFOS[1]])) {
              $AUTH_ROLES = $AUTH_TOKEN[$INFOS[1]];
          } else {
              $AUTH_ROLES = array($AUTH_TOKEN[$INFOS[1]]);
          }
      if (count($INFOS)>2 && is_array($INFOS[2]) && count($INFOS[2])>0 && count(array_intersect($INFOS[2], $AUTH_ROLES))==0) {
          header('HTTP/1.0 403 Forbidden');
          exit;
      }
      $USER = in_array($AUTH_USER, $ADMINUSERS) ? $ADMINGROUP : $AUTH_USER;
      break;
   }

   break;

} while(0);

?>
