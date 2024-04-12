<?php

#docker mode : 0 = FALSE; 1 = TRUE;
$docker_mode = 1;

// Range of years for the Maggot project
$YEAR='2022-'.date("Y");

// Zoom out webpage
$ZOOMWP = '90%';

// Add a media type for each resource
$RESMEDIA=1;

// Private access key management
$PRIVATE_ACCESS=0;

// Waiting time while loading vocabulary via Ajax API
$AJAXWAIT=5000;

#------------------------

// Mode external (1) or not (0)
$EXTERN=0;

// Main title & title 
$TITLE = 'Metadata management ';
$TITLE .= ( isset( $_GET['extern'] ) || $EXTERN ) ? 'for Datasets' : 'within storage spaces';
$MAINTITLE =$TITLE;

// File Browser
$FILEBROWSER=0;
$URL_FILEBROWSER='/fb/';

// Link to a FAIR website
$FAIRLINK=1;
$URL_FAIR="https://scholar.harvard.edu/files/mercecrosas/files/fairdata-dataverse-mercecrosas.pdf";

#------------------------

// Get http protocol
if (isset($_SERVER['HTTP_X_FORWARDED_PROTO']) && $_SERVER['HTTP_X_FORWARDED_PROTO'] == 'https') {
   $HTTP = 'https://';
} else {
   $HTTP = 'http://';
}

// Get the URL of the application
$APPNAME='maggot';
$HOSTNAME=$_SERVER['HTTP_HOST'];
if (isset($_SERVER['HTTP_X_APP_NAME']) && strlen($_SERVER['HTTP_X_APP_NAME']))
   $APPNAME=$_SERVER['HTTP_X_APP_NAME'];

$APPURL=$HTTP.$HOSTNAME.'/'.$APPNAME;

#------------------------

// JSON schema
$url_schema = $APPURL."/conf/maggot-schema.json";

// Dataverse URL
$dataverse_urls = array('https://demo.recherche.data.gouv.fr',
                        'https://data-preproduction.inrae.fr',
                        'https://entrepot.recherche.data.gouv.fr');
$DV_API = '/api/search';

$SERVER_URL='https://entrepot.recherche.data.gouv.fr';
$PARENT='';


// Zenodo URLs
$zenodo_urls = array('https://zenodo.org',
					 'https://zenodo-rdm-qa.web.cern.ch');
//                     'https://sandbox.zenodo.org');

$ZC_API = '/api/communities/';

$ZENODO_SERVER_URL='https://zenodo.org';
$ZENODO_API_TOKEN='';
$COMMUNITIES='';

// OAI-PMH
$OAI_HTTP = 'http';
$OAI_BASEURL=$OAI_HTTP.'://'.$APPURL.'/oai';
$OAI_EMAIL='pfmb-bordeaux@inra.fr';
$OAI_IDENTIFIER='pmb-bordeaux.fr';
$OAI_SET='maggot';
$OAI_DATE='2023-01-01';

// Help link (online documentation on GitHub)
$maggot_doc_url="https://inrae.github.io/pgd-mmdt/";

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
$export_oai = 0;

#------------------------

// EXPERIMENTAL : Bloxberg Blockchain
$BB_PUBLICKEY='';
$BB_API_TOKEN='';
$bloxberg_urls = array('https://certify.bloxberg.org/');
$export_bloxberg = 0;

// Name of the file that must be present in the data directory to authorize 
// the transfer of the metadata file to the Bloxberg Blockchain
$auth_bloxberg_file = 'META_bloxberg_ok.txt';

#------------------------

// Vocabulary cache file
$vocab_cache_file = 'cache/vocabulary.txt';
$nofound_cache_file = 'cache/nofound.txt';

// Directory of the Control Vocabulary Listes (cvlist)
$cvdir='cvlist/';

// Maggot name of the field corresponding to the title in dataverse/zenodo
$maggot_fulltitle='fulltitle';

//Name of the file that must be present in the data directory to authorize the transfer of the data file
$auth_senddata_file = 'META_datafile_ok.txt';

// Name of the private access file
$private_auth_file = 'META_auth.txt';

// Some links (see inc/schema/schema.inc)
$url_raw_licences = 'https://raw.githubusercontent.com/spdx/license-list-data/main/json/licenses.json';
$url_ror = 'https://api.ror.org/organizations?query=';

#------------------------

// Configuring messaging for sending metadata to data managers
$sendMail=NULL;

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
$DEV=1;
$LOG=0;
if (is_file('inc/config/dev.inc'))
    include ('inc/config/dev.inc');

?>
