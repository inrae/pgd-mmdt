<?php

#------------------------
// Mongo DB
#------------------------
#db credentials
$username =  rawurlencode('userr-pgd');
$password =  rawurlencode('rrrrr');
$email_admin = "admin@your-domain.org";

$database = 'pgd-db';
$collection = 'metadata';
$port = '27017';

$uritarget = '127.0.0.1';
if ($docker_mode==1){$uritarget='mmdt-db';}
$uri = "mongodb://$username:$password@$uritarget:$port/$database";
$client = new MongoDB\Driver\Manager($uri);

?>
