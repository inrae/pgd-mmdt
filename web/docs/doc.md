# Metadata documentation
----

.

## DEFINITION

* This section allows you to describe shortly your dataset.  See [Documentation on Data management](https://libraries.mit.edu/data-management/store/documentation/).


<div class="decal" markdown="1">
### Short name

* Give your dataset a short name that can be seen as a unique persistent identifier or a name that will serve as a reference such as a project name. This name will then be mainly used to identify it and reference the metadata page via a URL. So it is not the main title of the dataset (see full title). Do not use spaces or non-alphanumeric characters. Only letters, numbers and underscores are allowed. The number of characters is limited to 20.


### Full title

* Concise and precise title of your dataset. It is this title that will appear on the data repository (Dataverse and Zenodo) as the main title. Can sometimes use the title of the publication if it is not too long.


### Subject

* The area of the study relevant to the dataset. The list of terms is imposed by Dataverse. It is also a mandatory field. Adding, modifying or deleting them may prevent uploading to the repository. See [OECD](https://web-archive.oecd.org/2012-06-15/138575-38235147.pdf) and [European Commission](https://joinup.ec.europa.eu/collection/eu-semantic-interoperability-catalogue/solution/field-science-and-technology-classification/about) about field of science and technology classification.


### Description of the dataset

* A summary describing the purpose, nature and the scope of the dataset. It is the equivalent of the abstract for an article.


### Notes

* You can add a note which will be added in the data repository. Typically, put here the reference to funders who do not have an identifier recognized in international registers. This field will be pushed into the data repository (Dataverse and Zenodo).


</div>

.
## STATUS

* This section allows you to define the status of the dataset, associated rights, dates, etc.


<div class="decal" markdown="1">
### Status of the dataset

* Choose a status for the dataset. This field is used for data management and will not subsequently be reported in the data repository (Dataverse or Zenodo).


    *  **Processed** :  Data are available and have been curated. Which means that the data is in a stable and redistributable version as is. Any dataset that must be published must have this status.

    *  **In progress** :  Some data is available but more is to come. Some are stable but others are not. Any dataset with this status must only be shared between partners of the same project.

    *  **Unprocessed** :  The data is available but not curated. Which means that the data is not yet stable and therefore cannot be distributed as is. Any dataset with this status must only be shared between partners of the same project.

### Access rights to data

* Indicates the status of the data regarding access and dissemination. This field is used for data management and will not subsequently be reported in the data repository (Dataverse or Zenodo).


    *  **Public** :  Either a link to the data is provided as a resource with password-free access (e.g filebrowser) or access to the data is possible to anyone with access to the storage space (locally or remotely via VPN).

    *  **Private** :  Access requiring specific rights (e.g. filebrowser or on storage space).

    *  **Mixte** :  Mix of public and private access

### Language

* The language in which the dataset files are documented.


### Life cycle step

* This concerns all stages of the data life cycle. Specifies the event happening over the data life cycle that is considered significant enough to document.  The list of terms is imposed by Dataverse. Adding, modifying or deleting them may prevent uploading to the repository. See [DDI -CV](https://ddialliance.org/Specification/DDI-CV/LifecycleEventType_1.0.html). <br>Note: It can be omitted if you don't know what to put.


### License

* License/Data Use Agreement. See  [Choose a license](https://choosealicense.com/licenses/) and [CC-BY licenses](https://creativecommons.org/share-your-work/cclicenses/). <br>(Note: [Etalab 2.0](https://www.etalab.gouv.fr/wp-content/uploads/2017/04/ETALAB-Licence-Ouverte-v2.0.pdf) is the recommended license in accordance with France's open data policy)


### Start of collection

* Start date of sample or data collection. A full date in [ISO-8601 format](https://en.wikipedia.org/wiki/ISO_8601) is required even if the month and day are fictitious. This date allows us to know when the implementation of the experimental process began.


### End of collection

* End date of sample or data collection. A full date in [ISO-8601 format](https://en.wikipedia.org/wiki/ISO_8601) is required even if the month and day are fictitious. This date allows you to know when data collection is complete.


### DMP identifier

* Preferably a URL pointing directly to the [DMP](https://en.wikipedia.org/wiki/Data_management_plan) but it can also be a permanent identifier (DOI, etc.). See [what-is-a-dmp-id](https://support.datacite.org/docs/what-is-a-dmp-id).


</div>

.
## MANAGEMENT

* This section allows you to assign names of people or organizations who participated in the production of data and according to the type of participation. Mainly based on [Datacite CV](http://purl.org/datacite/v4.4/ContributorType)


<div class="decal" markdown="1">
### Contacts

* Person that users of the dataset can contact with questions. Based on the [People](../dictionary?dico=people) dictionary. Note that people defined as contact must have an email address defined via the dictionary.


### Authors

* Person that created the dataset. Based on the [People](../dictionary?dico=people) dictionary.


### Data collectors

* Person responsible for finding, gathering/collecting data under the guidelines of the author(s) or Principal Investigator (PI). Based on the [People](../dictionary?dico=people) dictionary.


### Data curators

* Person who organizes, integrates, and annotates data collected from various sources in order that the value of the data is maintained over time and the data remains available for reuse and preservation. Based on the [People](../dictionary?dico=people) dictionary.


### Project members

* Person on the membership list of a designated project/project team.. Based on the [People](../dictionary?dico=people) dictionary.


### Project leader

* Person officially designated as head of project team or sub- project team instrumental in the work necessary to development of the resource. Based on the [People](../dictionary?dico=people) dictionary.


### WP leader

* The Work Package Leader is responsible for ensuring the comprehensive contents, versioning, and availability of the Work Package during the development of the resource. Based on the [People](../dictionary?dico=people) dictionary.


### Depositor

* The entity, e.g. a person or organization, that deposited the dataset in the repository.


### Producer

* The entity that serves to produce the dataset. Based on the [Producer](../dictionary?dico=producer) dictionary.


### Grant Information

* Information about the dataset's financial support. Based on the [Grant](../dictionary?dico=grant) dictionary.


</div>

.
## DESCRIPTORS

* This section allows you to define elements characterizing the data themselves and certain experimental conditions for obtaining them.


<div class="decal" markdown="1">
### Keywords

* A key term that describes an important aspect of the dataset and information about any controlled vocabulary used. Based on [BioPortal ontologies](https://bioportal.bioontology.org/ontologies) : EFO, JERM, EDAM, MS, NM, NCI, OBI, PO, PTO, AGRO, ECOCORE, IOBC, NCBITAXON.


### Topic Classification

* Indicates a broad, important topic or subject that the dataset covers and information about any controlled vocabulary used. Based on [Thesaurus-INRAE](https://thesaurus.inrae.fr/thesaurus-inrae/en/).


### Kind of Data

* The type of data included in the files (e.g. survey data, machine-readable text, experimental data tables). The list of terms is imposed by Dataverse. Adding, modifying or deleting them may prevent uploading to the repository. See [DDI-CV](https://ddialliance.org/Specification/DDI-CV/KindOfDataFormat_1.0.html)


### Data origin

* Origin of the data. The list of terms is imposed by Dataverse. Adding, modifying or deleting them may prevent uploading to the repository.


### Experimental Factor

* Specify experimental factors i.e controlled independent variable. Based on the [Vocabulary](../dictionary?dico=vocabulary) dictionary.


### Measurement type

* Specify the types of measurements carried out, e.g metabolites, phenotypic data, environmental data, etc. Based on the [Vocabulary](../dictionary?dico=vocabulary) dictionary.


### Technology type

* Specify the types of instrument used to carry out all or part of the measurements, e.g NMR, LC-MS, ... Based on the [Vocabulary](../dictionary?dico=vocabulary) dictionary.


### Publication - Citation

* The full bibliographic citation for the related publication


### Publication - ID Type

* The type of the identifier that uniquely identifies a related publication. Based [Datacite CV](http://purl.org/datacite/v4.4/RelatedIdentifierType).


    *  **ark,arXiv,bibcode,doi,ean13,eissn,handle,isbn,issn,istc,lissn,lsid,pmid,purl,upc,url,urn** :  

### Publication - ID Number

* The identifier for a related publication corresponding to the specified identifier type.


### Publication - URL

* The URL of the publication web page, e.g. a journal article webpage


</div>

.
## OTHER

* This section allows you  to enter miscellious information (protocols, comments, issues, ...)


<div class="decal" markdown="1">
### Additional information

* Although it is preferable to put all data documents online via appropriate repositories (e.g. [protocols.io](https://www.protocols.io/) for protocols) or in an electronic laboratory notebook (e.g [eLabFTW](https://www.elabftw.net/)), you can nethertheless add any comments concerning the data which could prove useful in their generation, in their interpretation, in their location, ...


</div>

.
## RESOURCES

* This section allows you to define all the resources you want. <br>- External resources will be specified by a URL with preference for a permanent identifier (e.g. DOI) but also any URL pointing to data whether they comply with the FAIR principle (e.g. [ODAM](https://inrae.github.io/ODAM/)) or not. <br>- Internal resources will be the data files to be uploaded to the data repository at push time. In the latter case the exact name of the file on the storage space must appear in the location field. <br>- Furthermore, in the case of local data management, it would be wise to indicate in which space the data is located if it is not located in the same place as the metadata (e.g. NextCloud, Unit NAS, etc.)


<div class="decal" markdown="1">
### Resource Type

* Choose the type of the resource. Based on [Datacite CV](http://purl.org/datacite/v4.4/ResourceTypeGeneral).


### Media Type

* Choose a media type if applicable. Based on [Datacite CV](https://bioportal.bioontology.org/ontologies/FDC-GDMT/?p=classes&conceptid=http%3A%2F%2Fvocab.fairdatacollective.org%2Fgdmt%2FMIMEType).


### Description

* Provide a concise and accurate description of the resource. Must not exceed 30 characters.


### Location

* Preferably indicate a URL to an external resource accessible to all. But it can also be a password-protected resource (e.g. a disk space on the cloud). This can also be text clearly indicating where the resource is located (internal disk space). Finally, this can be the name of a file deposited on the same disk space as the metadata file, in order to be able to push it in the data repository at the same time as the metadata (see [Publication](https://inrae.github.io/pgd-mmdt/publish/) on the online documentation).


</div>

.
