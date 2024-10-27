---
title: Maggot - Configuration settings
summary: 
authors:
    - Daniel Jacob
date: 2023-10-09
some_url:
---

# Configuration settings

<style>.md-typeset h1 {display: none;} .md-nav__item {font-size: medium}</style>

<style>.md-typeset table:not([class]) th {
    background-color: var(--md-default-fg-color--light);
    color: var(--md-default-bg-color); min-width: 5rem; padding: 0.9375em 1.25em; vertical-align: top;}</style>

### Configuration settings

Here is the list of all files that may be subject to adjustment of certain parameters according to the needs of the instance site.

<br>

#### dockerscanpart/scripts/config.py

This file defines the connection parameters to the Mongo database. Knowing that this database is only accessible internally, in principle they do not need to be changed.

***Note***: *These settings must be the same as defined in [dockerdbpart/initialisation/setupdb-js.template][1]{:target="_blank"}*

|Parameter | Description | Default value
------------ | ------------- | ------------ 
dbserver | Name of the MongoDB server | mmdt-db
database | Name of the MongoDB database | pgd-db
dbport | Port of the MongoDB server | 27017
username | Username of the Mongo database pgd-db with Read/Write access | userw-pgd
password | Password corresponding to the username of the Mongo DB pgd-db | wwwww

<br>

#### inc/config/mongodb.inc

This file defines the connection parameters to the Mongo database. Knowing that this database is only accessible internally, in principle they do not need to be changed.

***Note***: *These settings must be the same as defined in [dockerdbpart/initialisation/setupdb-js.template][1]{:target="_blank"}*

|Parameter | Description | Default value
------------ | ------------- | ------------ 
|docker_mode | Indicates whether the installation involves using docker containers. In this case, the Mongo DB IP address will be different from 127.0.0.1. | 1
uritarget | the Mongo DB IP address | mmdt-db (docker_mode=1) or 127.0.0.1 (docker_mode=0)
database | Name of the MongoDB database | pgd-db
collection | Name of the MongoDB collection | metadata
port | Port of the MongoDB server | 27017
username | Username of the Mongo database pgd-db with Read access only | userr-pgd
password | Password corresponding to the username of the Mongo DB pgd-db | rrrrr

<br>

#### inc/config/config.inc

This file defines parameters related to *i*) the web interface, *ii*) the functionalities allowed for users. Only the parameters that could be useful to be changed for the needs of an instance are described here.

|Parameter | Description | Default value
------------ | ------------- | ------------ 
EXTERN | Indicates if the use of the tool is only for external use, i.e. without using a storage space. | 0
PRIVATE_ACCESS | Gives the possibility of managing private access to metadata | 0
ZOOMWP | Zoom level regarding the web interface. By reducing the size slightly, you get a better layout. | 90%
RESMEDIA | Gives the possibility of putting a MINE type on each resource in the metadata. | 1
TITLE | Title to display in main banner | Metadata management
FILEBROWSER | Indicates whether the file browser is used. This assumes it is installed. | 0
URL_FILEBROWSER | File browser URL. It can be absolute or relative. | /fb/
APPNAME | Name given in the URL to access the web interface. | maggot
dataverse_urls | Array of Dataverse repository URLs where you can upload metadata and data | -
zenodo_urls | Array of Zenodo repository URLs where you can upload metadata and data | -
SERVER_URL | Default Dataverse repository URL | https://entrepot.recherche.data.gouv.fr
ZENODO_SERVER_URL | Default Zenodo repository URL | https://zenodo.org
export_dataverse | Indicates whether the Dataverse feature is enabled | 1
export_zenodo | Indicates whether the Zenodo feature is enabled | 1
export_jsonld | Indicates whether the JSON-LD feature is enabled | 1
export_oai | Indicates whether the OAI-PMH feature is enabled | 0
export_bloxberg | Indicates whether the Bloxberg Blockchain feature is enabled (Experimental) | 0
cvdir | Relative path of the Control Vocabulary Listes (cvlist) | cvlist/
maggot_fulltitle | Maggot name of the field corresponding to the title in dataverse/zenodo | fulltitle
auth_senddata_file | Name of the file that must be present in the data directory to authorize the transfer of the data file | META_datafile_ok.txt
private_auth_file | Name of the private access file | META_auth.txt
sendMail | Configuring messaging for sending metadata to data managers (see below) | NULL

<br>

The messaging configuration is done using the following array in the **inc/config/config.inc** file (or more judiciously in **inc/config/local.inc** in order to be preserved during an update) - To understand how it works see [Send Emails using PHPmailer][2]{:target="_blank"}

```php
$sendMail['smtpHost'] = 'smtp.example.org';        //  Set the SMTP server to send through
$sendMail['smtpSecure'] = 'tls';                   //  Enable TLS encryption
$sendMail['smtpPort'] = 587;                       //  Set the TCP port to connect to
$sendMail['CheckEmail'] = 'maggot@exemple.org';    //  Email address authorized to send emails
$sendMail['CheckPass'] = 'password';               //  The corresponding password
$sendMail['CheckName'] = 'Maggot';                 //  Alias name
$sendMail['UserEmail'] = 'admin@exemple.org';      //  Email of data managers, separated by a comma
```

<br>

#### run

This file contains the essential parameters to be set before any use.

|Parameter | Description | Default value
------------ | ------------- | ------------ 
WEB_PORT | Local HTTP Port for web application | 8087
DATADIR | Path to the data | /opt/data/
DB_IMAGE | Docker image name of the MongoDB | pgd-mmdt-db
SCAN_IMAGE | Docker image name of the Scan process | pgd-mmdt-scan
WEB_IMAGE | Docker image name of the Web interface | pgd-mmdt-web
DB_CONTAINER | Docker container name of the MongoDB | mmdt-db
SCAN_CONTAINER | Docker container name of the Scan process | mmdt-scan
WEB_CONTAINER | Docker container name of the Web interface | mmdt-web
MONGO_VOL | Volume name for MongoDB | mmdt-mongodb
MONGO_PORT | HTTP Port for MongoDB | 27017
USER | Admin user in the htpasswd file | admin

<br><br>

[1]: https://github.com/inrae/pgd-mmdt/blob/main/dockerdbpart/initialisation/setupdb-js.template
[2]: https://wpwebinfotech.com/blog/send-emails-using-php/