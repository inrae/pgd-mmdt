<?php

session_start([ 'cookie_lifetime' => 3600,  'gc_maxlifetime' => 3600]);

date_default_timezone_set('Europe/Paris');
ini_set('error_reporting', E_ERROR);
ini_set("include_path", ".:/usr/share/php");
ini_set("display_errors", 1);

#docker mode : 0 = FALSE; 1 = TRUE;
$docker_mode = 1;

// Range of years for the Maggot project
$YEAR='2022-'.date("Y");

// Zoom out webpage
$ZOOMWP = '90%';

// Add a media type for each resource
$RESMEDIA=1;

// Online Configuration 
$CONFIGURATION=1;

// Private access key management
$PRIVATE_ACCESS=0;

// Waiting time while loading vocabulary via Ajax API
$AJAXWAIT=3000;

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

// Get the URL of the application
$APPURL = 'http://localhost/maggot';
if (file_exists('conf/HOST')) $APPURL=str_replace("\n", '', file_get_contents("conf/HOST"));

#------------------------

$VERSION='1.0';
if (file_exists('./conf/VERSION')) $VERSION = file_get_contents('./conf/VERSION');

#------------------------

// JSON schema
$url_schema = $APPURL.'/conf/maggot-schema.json';

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
$OAI_BASEURL=str_replace('https', 'http', $APPURL).'/oai';
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

// ORCID API parameters
$ORCID_CLIENT_ID = 'APP-81Z4YTAMWUMD62N2';
$ORCID_CLIENT_SECRET = '45eb72e7-84ef-455a-8016-bfdde2527861';
$ORCID_OAUTH_URL = 'https://orcid.org/oauth';
$ORCID_API_URL = 'https://pub.orcid.org/v3.0/csv-search';

#------------------------

// Configuring messaging for sending metadata to data managers
$SENDMAIL=NULL;

#------------------------

# Name of the group with administrator rights (e.g dictionaries)
$ADMINGROUP = 'admin';

# Userinfo endpoints for each Identity Provider
$AUTH_IP = NULL;

# List of admin users (login names)
$ADMINUSERS = array();

#------------------------

// URL of the JSON Schema Validation API
// see https://assertible.com/json-schema-validation
$JSVAPI_URL="https://assertible.com/json";

#------------------------

require_once('inc/config/mongodb.inc');

#------------------------

// For development version only
$DEV=1;
$LOG=0;

// If local configuration
if (is_file('inc/config/local.inc'))
    include ('inc/config/local.inc');

#------------------------

include ('inc/functions/auth.inc');

?>