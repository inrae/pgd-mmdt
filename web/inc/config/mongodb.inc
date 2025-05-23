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

$uritarget='mmdt-db';
$uri = "mongodb://$username:$password@$uritarget:$port/$database";
$client = new MongoDB\Driver\Manager($uri);

?>
