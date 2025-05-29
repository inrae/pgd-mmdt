<?php

# Nom du fichier: index.php
# Auteur(s): D.JACOB 
# Copyright: (c) INRAE - 2015-2021
#
# Loading config file
require_once('inc/config/config.inc');
include ('inc/parsedown/Parsedown.inc');
include ('inc/parsedown/ParsedownExtra.inc');

$home_file='docs/home.md';

$PATH='';

include ('inc/header.inc');
include ('inc/doc/home.inc');
include ('inc/footer.inc');

?>
