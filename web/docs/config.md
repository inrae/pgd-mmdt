# Configuration
----

.

## Overview of the configuration of the PGD-MMDT tool

.

* There are two configuration files to set up.

   * **The terminology definition file** (***config_terms.txt***) serving to describe all terminology used to define the metadata of a dataset.
   * **The terminology documentation file** (***config_doc.txt***) serving to documente all terminology definitions.

.

![Alt text](../docs/config.png?raw=true "Overview of the configuration")

.

* **Note** : The step numbers shown in the figure above are mentioned in brackets in the text below.
.

.

## TSV to JSON

* This function is used to generate the terminology definition file in JSON format (***config_terms.json***) and the corresponding JSON-Schema file (***pgd-mmdt-schema.json***) from a tabulated file (**1**). You can either create a terminology definition file in TSV format from scratch (see below to have more details), or extract the file from the current configuration (see JSON to TSV).

* Once the terminology definition file has been obtained (**2**), you can load it and press 'Submit'.

* Three files are generated (**3** & **5**):
   * **config_terms.json** and **pgd-mmdt-schema.json** : These files should be placed in the web/json directory (**3**). A (re)start of the application must be done in full mode (**4**) (***sh ./run fullstart***)
   * **config_doc.txt** (**5**) : This file serves as a template for the documentation of the metadata profile. You should edit it with a spreadsheet program, and fill in the description column (**6**). Then it is used to generate the documentation file in markdown format (see TSV to DOC).

.

## TSV to DOC

* This function generates the markdown documentation file (***doc.md***) from the template file (***config_doc.txt***) which is itself generated from the metadata definition file (***config_terms.txt***, cf TSV to JSON).

* Once the template file for the documentation (***config_doc.txt***) has been edited and documented (**6**) (see below to have more details), you can load it and press 'Submit'.

* The documentation file in markdown format (***doc.md***) is thus generated (**7**) and must be placed in the web/docs directory (**8**). Users will have access to this documentation file via the web interface, in the documentation section, heading "Metadata".

.

## JSON to TSV

* This function allows you to extract the terminology definition file in TSV format (***config_terms.txt***) from the current configuration. This allows you to start from this file, either to adapt your own metadata profile or simply to modify it slightly.

.

---

.

## Definition of terminology
.

Terminology is the set of terms used to define the metadata of a dataset. A single file (***web/json/config_terms.txt***) contains all the terminology. It is therefore necessary to create and fill it, according to the following scheme:

* Terminology is organised in several sections. By default 6 sections are proposed:

    - ***DEFINITION*** : section for defining the short name of projet.
    - ***STATUS*** : section to indicate the status of the dataset and its retention period
    - ***MANAGEMENT*** : section for entering the metadata relating to the management of the dataset (access rights, scientific and technical management, end-users, contracts, valorisation)
    - ***LOCATION*** : section allowing to enter the metadata related to the device/site and/or geographical location from which the dataset comes (Facility, Site, City)
    - ***DESCRIPTORS*** : section for entering metadata about the (biological) description of the dataset and the type of data it contains.
    - ***RESOURCES*** : section for entering metadata about some resources, i.e description of data included in the dataset. This section is mandatory.

* For each section, fields are then defined. These fields can be defined according to the way they will be entered via the web interface. There are 4 different types of input: check boxes (***checkbox***), drop lists (***dropbox***), single line text boxes (***textbox***) and multi-line text boxes (***areabox***).

* For two types (***checkbox*** and ***dropbox***), it is possible to define the values to be selected (predefined terms).

.

### Structure of the Terminology definition file (TSV)

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
   3. **autocomplete** : In this release, we have implemented the possibility to put vocabulary from BioPortal (https://bioportal.bioontology.org/). See the [bponto.js](https://github.com/inrae/pgd-mmdt/blob/main/web/js/autocomplete/bponto.js) script as an example of usage. In addition, 3 other lists of names for autocompletion are available: 'cities', 'regions' and 'departments' for France, all obtained using the [geo.api.gouv.fr](https://geo.api.gouv.fr/decoupage-administratif) API. (e.g autocomplete=[cities](https://github.com/inrae/pgd-mmdt/blob/main/web/js/autocomplete/bponto.js) for selecting a french city). You are free to implement other lists, either static or involving APIs.
   4. A special field that must be named ***resources*** and belong to the ***resources*** section can be added to the list of fields (see the example below). This 'resources' field will allow you to add via the web interface, a list of internal or external resources with their type, description and location. A location can be anything: a text, an absolute path in a tree, a URL link, ... You can thus put a link to a publication: Type=article, link=DOI. 

.

Below an example of Terminology definition file (TSV)

![Alt text](../docs/config_terms_txt.png?raw=true "Example of Terminology definition file")

.

### Structure of the Terminology documentation file (TSV)

The file must have 3 columns with headers defined as follows:

* column 1 - **Type** : The type of the element, namely 'section', 'field' or 'option'. An 'option' type must correspond to each of the options for a field corresponding to a drop-down list.
* column 2 - **Name** : Name of the element. The names of the sections, variables and drop-down options must be exactly the same as those specified in the terminology definition file.
* column 3 - **Description** : The description corresponding to the element, serving as much as possible to give indications on the information to be selected or entered, in order to remove possible ambiguities.

.

Below an example of Terminology documentation file (TSV)

![Alt text](../docs/config_doc_txt.png?raw=true "Example of Terminology documentation file")

.

## Links

* **Source code on Github** : <https://github.com/inrae/pgd-mmdt>


## Developers

* François Ehrenmann ([@UMR BioGECO][1]) | CATI [@GEDEOP][5]

* Philippe Chaumeil ([@UMR BioGECO][1])

* Daniel Jacob ([@UMR BFP][2]) | CATI [@PROSODIe][4]

.

## Contributors

* Edouard Guitton ([@INRAE Dept. SA][6], [@Emerg'IN][3])

[1]: https://www6.bordeaux-aquitaine.inrae.fr/biogeco_eng/
[2]: https://www6.bordeaux-aquitaine.inrae.fr/bfp_eng/
[3]: https://www.emergin.fr/emergin_eng/
[4]: https://prosodie.cati.inrae.fr/
[5]: https://gedeop.cati.inrae.fr/
[6]: https://www.inrae.fr/departements/sa

.

*[Schema]: vocabulary that allows you to annotate and validate JSON documents.
*[JSON]: JavaScript Object Notation : format used to represent structured information
*[TSV]: open text format representing tabular data as "tab-separated values". Each row corresponds to a table row and the cells in a row are separated by a tab.
*[markdown]: Markdown is a lightweight markup language designed to provide easy-to-read and easy-to-write syntax. A Markdown document can be read as is without appearing to have been marked up or formatted with special instructions.
*[autocomplete]: feature in which an application predicts the rest of a word a user is typing

