<?php

# Nom du fichier: index.php
# Auteur(s): D.JACOB 
# Copyright: (c) INRAE - 2015-2021
#
session_start([ 'cookie_lifetime' => 3600 ]);

date_default_timezone_set('Europe/Paris');
ini_set('error_reporting', E_ALL & ~E_NOTICE);
ini_set("include_path", ".:/usr/share/php");
ini_set("display_errors", 1);

require_once('inc/config/config.inc');
include ('inc/main.inc');

?>
