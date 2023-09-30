![Logo](web/docs/logo.png)

[![GitHub release](https://img.shields.io/badge/release-1.1-blue)](https://github.com/inrae/pgd-mmdt/releases/tag/1.1)

### Metadata Management Tool for Data Storage Spaces


**MAGGOT** is a metadata management tool that addresses the problems of organising, documenting, storing and sharing data in a research unit or infrastructure, and fits perfectly into a structural data management plan.

This tool is built around a web interface coupled with a MongoDB database.

The web interface allows you to :

- **Describe** a dataset using metadata of different types by facilitating their entry and avoiding their re-entry thanks to personalized dictionaries (Description)
- **Search** datasets by their metadata (Accessibility)
- **Publish** the metadata of datasets along with their data files into the Recherche Data Gouv (https://recherche.data.gouv.fr/en) repository (Publication)

This tool allows you to manage the metadata of any dataset. To do this:

* In a first step, you enter the metadata concerning the dataset using the web interface. For selection purposes, either a set of terms can be predefined, or terms can be added to the list and stored for future use. 
* As output, you retrieve the generated JSON file, then drop it into the corresponding data directory.
* In order for this metadata file to be taken into account, you either launch a scan manually, or wait for the next automatic scan to be triggered (via cron).
* Finally, you can search for your datasets using a dedicated web interface to find them by specifying one or more criteria. 

An overview is given by the figure below:

![Alt text](web/docs/overview.png?raw=true "Overview")


See our online instance of the Maggot tool : https://pmb-bordeaux.fr/maggot/

------

### Install on your linux computer or linux / unix server

Requirements: a recent OS that support Docker (see https://www.docker.com/)

#### Retrieving the code
Go to the destination directory of your choice then clone the repository and `cd` to your clone path:

```sh
git clone https://github.com/inrae/pgd-mmdt.git pgd-mmdt
cd pgd-mmdt
```

#### Installation of Docker containers

MAGGOT uses 3 Docker images for 3 distinct services:

- **pgd-mmdt-db** which hosts the mongoDB database
- **pgd-mmdt-scan** which scans the data and updates the contents of the database and the web interface
- **pgd-mmdt-web** which hosts the web server and the web interface pages

#### Configuration

-  ***run*** : define web port, root of the data directory (including for development)
-  ***dockerdbpart/initialisation/setupdb-js.template*** : define MongoDB settings
-  ***dockerscanpart/scripts/config.py*** : define MongoDB settings (dbserver, dbport, username, password)
-  ***web/inc/config/config.inc*** : define MongoDB settings and also the URL for the JSON schema (see below)

***Warning*** : You have to pay attention to put the same MongoDB settings in all the above configuration files. It is best not to change anything. It would have been preferable to put a single configuration file but this was not yet done given the different languages involved (bash, javascript, python, PHP). To be done!

***Note*** : If you want to run multiple instances, you will need to change in the ***run*** file, *i)* the container names, *ii)* the data path, *iii)* the port number and the MongoDB volume name.

The following two JSON files are defined by default but can be easily configured from the web interface. See Terminology definition file in TSV.
-  ***web/conf/config_terms.json*** : define the terminology (see below)
-  ***web/conf/maggot-schema.json*** : define the JSON schema used to validate metadata files (see below)


#### Commands 

The ***run*** shell script allows you to perform multiple actions by specifying an option :

```sh
cd pgd-mmdt
sh ./run <option>
```
**Options**:
- ***build*** : Create the 3 Docker images namely ***pgd-mmdt-db***, ***pgd-mmdt-scan*** and ***pgd-mmdt-web***
- ***start*** : 1) Launch the 3 services by creating the Docker containers corresponding to the Docker images; 2) Create also the MongoDB volume.
- ***stop*** :  1) Remove all the 3 Docker containers; 2) Remove the MongoDB volume.
- ***initdb*** : Create and initialize the Mongo collection
- ***scan*** : Scan the data and update the contents of the database and the web interface
- ***fullstart*** : Perform the 3 actions ***start***, ***initdb*** and ***scan***
- ***restart*** : Perform the 2 actions ***stop*** then ***fullstart***
- ***ps*** : Check that all containers are running correctly
- ***passwd*** <_user_>: Define the admin password if no _user_ is specified, allowing you to copy the new configuration file on the server via the web interface (see configuration above) and to add entries in dictionaries. If a _user_ is specified, the dictionary consultation will be authorized for this user.

#### Starting the application

* You must first build the 3 docker container images if this has not already been done, by :
   ```sh
   sh ./run build
   ```

* The application can be sequentially started :
   * Starting the web interface
     ```sh
     sh ./run start
     ```
   * Initialization of the MongoDB database
     ```sh
     sh ./run initdb
     ```
   * Scanning the data directory for metadata files (META_XXXX.json)
     ```sh
     sh ./run scan
     ```

* You can also launch these 3 steps with a single command:
   ```sh
   sh ./run fullstart
   ```

#### Stoping the application

* To stop the application :
   ```sh
   sh ./run stop
   ```


------

### Architecture diagram

![Alt text](web/docs/schema.png?raw=true "Architecture diagram")

------

### Definition of terminology

Terminology is the set of terms used to define the metadata of a project. A single file (***web/conf/config_terms.json***) contains all the terminology. It is therefore necessary to configure it, according to the following scheme:

* Terminology is organised in several sections. By default 6 sections are proposed:

    - ***DEFINITION*** : section for defining the short name of projet.
    - ***STATUS*** : section to indicate the status of the dataset and its retention period
    - ***MANAGEMENT*** : section for entering the metadata relating to the management of the dataset (access rights, scientific and technical management, end-users, contracts, valorisation)
    - ***LOCATION*** : section allowing to enter the metadata related to the device/site and/or geographical location from which the dataset comes (Facility, Site, City)
    - ***DESCRIPTORS*** : section for entering metadata about the (biological) description of the dataset and the type of data it contains.
    - ***RESOURCES*** : section for entering metadata about some resources, i.e description of data included in the dataset. This section is mandatory.

* For each section, fields are then defined. These fields can be defined according to the way they will be entered via the web interface. There are 4 different types of input: check boxes (***checkboite***), drop lists (***listboite***), single line text boxes (***txtboite***) and multi-line text boxes (***areaboite***).

* For two types (***checkboite*** and ***listboite***), it is possible to define the values to be selected (***chkbxdico*** and ***listdico*** respectively).

* The terminology file (***web/conf/config_terms.json***) is structured as follows:

    -  ***sections*** : defines the sections (*labels*) and the fields within each section.
    -  ***sections_search*** : same as *sections* but only for researching data sets. So for example the "definition" section is not needed.
    -  ***shortview*** : defines the fields serving for the overview table after the search.
    -  ***checkboite***, ***listboite***, ***txtboite***, ***areaboite*** : defines the fields according to the way they will be entered via the web interface (i.e. check boxes, drop lists, single line text boxes and multi-line text boxes  respectively)

    -  ***chkbxdico***, ***listdico*** : defines for each field of type *checkboite* or *listboite*, the list of predefined terms. The *open* attributes indicates if it is possible to extend the list of terms via the web interface or not.

* Note that the ***title*** and ***description*** fields are mandatory.

* In the ***description*** field, you must add '_\_NL\__' if you want line breaks to be visible in the display.

------

### Terminology definition file in Tabulation-Separared-Values (TSV)

In order to facilitate the configuration task, it is possible to define the whole terminology using a spreadsheet. The file should be exported in TSV (Tabulated-Separated-Values) format.

* Just after the installation stage, you can perform a simple start (_sh ./run start_),
* Define the admin password allowing you to copy the new configuration file on the server via the web interface (_sh ./run passwd_)
* then in your browser at ***http://\<your-host\><:port>/config***, you could generate the corresponding ***config_terms.json*** file along with the JSON schema file ***pgd-mmdt-schema.json*** used to validate metadata files.
* Click on the '***Replace the current JSON files***' button to replace current JSON files under ***./web/conf***. A password will be asked to you to perform this replacement. This also assumes that you have write rights to these files. In case you have no write rights to these files or an _Internal Server Error_ occurs, you must put these files manually under ***./web/conf*** by yourself.
* You then restart in 'fullstart' mode (_sh ./run stop_ then _sh ./run fullstart_).
* You can then start to create your metadata files in JSON format, then put them in the corresponding data directories so that they can be scanned and finally found via the web interface.

#### Structure of the Terminology definition file (TSV)

The file must have 8 columns with headers defined as follows:

* column 1 - **Field** : shortname of the fields
* column 2 - **Section** : shortname ot the sections
* column 3 - **Search** : indicates if the field can be used as a criterion search ('Y') or not ('N')
* column 4 - **Shortview** : indicates with ordered numbers if the field serves for the overview table after the search (empty by default)
* column 5 - **Type** : indicates the way they will be entered via the web interface (possible values are: **textbox**, **dropbox**, **checkbox** and **areabox**).
* column 6 - **Features** : dependings on the Type value, one can specifiy some specific features. If several features, they must be separated by a comma.
     * for **checkbox**: open=0 or open=1 indicates if the selection is opened or not
     * for **textbox** & **checkbox**: autocomplete=*items* : The *items.js* file must be present under web/js/autocomplete. See note 3 below.
     * for **textbox** & **dropbox**: width=NNNpx allows you to specify the width of the box. Usefull if you want put several fields in the same line. See note 1 below.
     * for **areabox**: row=NN and cols=NN allows you to specify the row and column size of the textarea.
* column 7 - **Label** : Labels corresponding to the fields that will appear in the web interface
* column 8 - **Predefined terms** : for fields defined with a type equal to 'checkbox' or 'dropbox', one can give a list of terms separated by a comma.

* **Notes**: 
   1. the fields will be displayed in the same order as in the file and by section. So if you want to specify several textboxes with particular sizes so that they are on the same line, they should belong to the same section and follow each other in the file in the same order.
   2. the ***title*** and ***description*** fields are mandatory but not necessarily in the same section.
   3. **autocomplete** : In this release, we have implemented the possibility to put vocabulary:
      * from BioPortal (https://bioportal.bioontology.org/). See the bponto.js script as an example of usage,
      * from Skosmos-based Thesaurus. See the VOvocab.js script as an example of usage,
	  * from the geo.api.gouv.fr API : 3 lists of names for autocompletion are available: 'cities', 'regions' and 'departments' for France (e.g autocomplete=cities for selecting a french city),
	  * You are free to implement other lists, either static or involving APIs.
   4. A special field that must be named ***resources*** and belong to the ***resources*** section can be added to the list of fields (see the example below). This 'resources' field will allow you to add via the web interface, a list of internal or external resources with their type, description and location. A location can be anything: a text, an absolute path in a tree, a URL link, ... You can thus put a link to a publication: Type=article, link=DOI. 
Below an example of Terminology definition file (TSV)

![Alt text](web/docs/config_terms_txt.png?raw=true "Example of Terminology definition file")

------

### Funded by:

* INRAE UMR 1202 BIOGECO, Biodiversité Gènes et Communautés
* INRAE UMR 1332 BFP, Biologie du Fruit & Pathologie

### License

GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007 - See the included LICENSE file.


