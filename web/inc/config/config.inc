<?php
#db credentials
$username =  rawurlencode('userr-pgd');
$password =  rawurlencode('rrrrr');
$email_admin = "admin@your-domain.org";

$filebrowser = 0;

#docker mode : 0 = FALSE; 1 = TRUE;
$docker_mode = 1;

# URL of the JSON Schema Validation API
# see https://assertible.com/json-schema-validation
$JSVAPI_URL="https://assertible.com/json";

#------------------------
$database = 'pgd-db';
$collection = 'metadata';
$port = '27017';

$uritarget = '127.0.0.1';
if ($docker_mode==1){$uritarget='mmdt-db';}
$uri = "mongodb://$username:$password@$uritarget:$port/$database";
$client = new MongoDB\Driver\Manager($uri);
#------------------------

$url_schema = "https://pmb-bordeaux.fr/maggot/json/pgd-mmdt-schema.json";


?>
