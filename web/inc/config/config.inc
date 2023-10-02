<?php

// Current year
$YEAR='2022-2023';

// Zoom out webpage
$ZOOMWP = '90%';

// Add a media type for each resource
$RESMEDIA=1;

// File Browser
$FILEBROWSER=0;

// Link to a FAIR website
$FAIRLINK=1;
$URL_FAIR="https://scholar.harvard.edu/files/mercecrosas/files/fairdata-dataverse-mercecrosas.pdf";

// Mode external (1) or not (0)
$EXTERN=0;

#docker mode : 0 = FALSE; 1 = TRUE;
$docker_mode = 1;

// Main title & title 
$TITLE = 'Metadata management ';
$TITLE .= ( isset( $_GET['extern'] ) || $EXTERN ) ? 'for Datasets' : 'within storage spaces';
$MAINTITLE =$TITLE;

#------------------------

// Get the URL of the application
$HOSTNAME=$_SERVER['HTTP_HOST'];
$APPNAME='maggot';
if (isset($_SERVER['HTTP_X_APP_NAME']) && strlen($_SERVER['HTTP_X_APP_NAME']))
   $APPNAME=$_SERVER['HTTP_X_APP_NAME'];

$APPURL=$HOSTNAME.'/'.$APPNAME;

#------------------------

// JSON schema
$url_schema = "https://pmb-bordeaux.fr/maggot/conf/maggot-schema.json";

// Dataverse URL
$dataverse_urls = array('https://demo.recherche.data.gouv.fr',
                        'https://data-preproduction.inrae.fr/',
                        'https://entrepot.recherche.data.gouv.fr');

$SERVER_URL='https://entrepot.recherche.data.gouv.fr';
$PARENT='';

// Zenodo URLs
$zenodo_urls = array('https://zenodo.org',
                     'https://sandbox.zenodo.org');

$ZENODO_SERVER_URL='https://zenodo.org';
$ZENODO_API_TOKEN='';

// Help links (Temporarily on nextcloud while waiting for online documentation on GitBook)
$dataverse_help = "https://nextcloud.inrae.fr/s/otBMwKZXK62KtMj";
$dictionary_help = "https://nextcloud.inrae.fr/s/mCjciEP687kbn8i";

// Mapping file
$mapping_conf= 'conf/mapping.txt';

// Dataverse
$dataverse_conf= 'conf/dataverse.txt';
$export_dataverse = 1;

// JSON-LD Schema 
$jsonld_conf = 'conf/schema-jsonld.txt';
$export_jsonld = 1;

// Zenodo 
$zenodo_conf= 'conf/zenodo.txt';
$export_zenodo = 1;

// OAI-PMH
$oaipmh_conf= 'conf/oai-pmh.txt';
$export_oai = 1;

$OAI_HTTP = 'http';
$OAI_BASEURL=$OAI_HTTP.'://'.$APPURL.'/oai';
//$OAI_EMAIL='pfmb-bordeaux@inra.fr';
$OAI_EMAIL='daniel.jacob@inrae.fr';
$OAI_IDENTIFIER='pmb-bordeaux.fr';

// Vocabulary cache file
$vocab_cache_file = 'cache/vocabulary.txt';
$nofound_cache_file = 'cache/nofound.txt';

// Directory of the Control Vocabulary Listes (cvlist)
$cvdir='cvlist/';

// Maggot name of the field corresponding to the title in dataverse
$maggot_fulltitle='fulltitle';

//Name of the file that must be present in the data directory to authorize the transfer of the data file
$auth_senddata_file = 'META_datafile_ok.txt';

// Some links (see inc/schema/schema.inc)
$url_raw_licences = 'https://raw.githubusercontent.com/spdx/license-list-data/main/json/licenses.json';
$url_ror = 'https://api.ror.org/organizations?query=';

#------------------------

// URL of the JSON Schema Validation API
// see https://assertible.com/json-schema-validation
$JSVAPI_URL="https://assertible.com/json";

#------------------------

require_once('inc/config/mongodb.inc');

#------------------------

// If local configuration
if (is_file('inc/config/local.inc'))
    include ('inc/config/local.inc');

// For development version only
$DEV=0;
$LOG=0;
if (is_file('inc/config/dev.inc'))
    include ('inc/config/dev.inc');

?>
