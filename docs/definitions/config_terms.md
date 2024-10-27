---
title: Maggot - Terminlogy Definition
summary: 
authors:
    - Daniel Jacob
date: 2023-10-09
some_url:
---

# Terminlogy Definition

<style>.md-typeset h1 {display: none;} .md-nav__item {font-size: medium}</style>

<style>.md-typeset table:not([class]) th {
    background-color: var(--md-default-fg-color--light);
    color: var(--md-default-bg-color); min-width: 5rem; padding: 0.9375em 1.25em; vertical-align: top;}</style>

### Example of a Terminlogy Definition file


|Field | Section | Required | Search | ShortView | Type | features | Label | Predefined terms
------------ | ------------- | ------------ | ------------ | ------------ | ------------- | ------------ | ------------ | ------------
|title | definition | Y | N | 1 | textbox | width=350px | Short name | 
|fulltitle | definition | Y | Y | 2 | textbox |  | Full title | 
|subject | definition | Y | Y |  | checkbox | open=0 | Subject | Agricultural Sciences,Arts and Humanities,Astronomy and Astrophysics,Business and Management,Chemistry,Computer and Information Science,Earth and Environmental Sciences,Engineering,Law,Mathematical Sciences,Medicine Health and Life Sciences,Physics,Social Sciences,Other
|description | definition | Y | Y |  | areabox | rows=6,cols=30 | Description of the dataset | 
|note | definition | N | Y |  | areabox | rows=4,cols=30 | Notes | 
|status | status | N | Y | 3 | dropbox | width=350px | Status of the dataset | Processed,In progress,Unprocessed
|access_rights | status | N | Y | 4 | dropbox | width=350px | Access rights to data | Public,Mixte,Private
|language | status | N | Y |  | checkbox | open=0 | Language | Czech,Danish,Dutch,English,Finnish,French,German,Greek,Hungarian,Icelandic,Italian,Lithuanian,Norwegian,Romanian,Slovenian,Spanish,Swedish
|lifeCycleStep | status | N | Y |  | multiselect | autocomplete=lifecycle,min=1 | Life cycle step | 
|license | status | N | Y |  | textbox | autocomplete=license,min=1 | License | 
|datestart | status | N | Y |  | datebox | width=350px | Start of collection | 
|dateend | status | N | Y |  | datebox | width=350px | End of collection | 
|dmpid | status | N | Y |  | textbox |  | DMP identifier | 
|contacts | management | Y | Y |  | multiselect | autocomplete=people,min=1 | Contacts | 
|authors | management | Y | Y |  | multiselect | autocomplete=people,min=1 | Authors | 
|collectors | management | N | Y |  | multiselect | autocomplete=people,min=1 | Data collectors | 
|curators | management | N | Y |  | multiselect | autocomplete=people,min=1 | Data curators | 
|members | management | N | Y |  | multiselect | autocomplete=people,min=1 | Project members | 
|leader | management | N | Y |  | multiselect | autocomplete=people,min=1 | Project leader | 
|wpleader | management | N | Y |  | multiselect | autocomplete=people,min=1 | WP leader | 
|depositor | management | N | Y |  | textbox |  | Depositor | 
|producer | management | N | Y |  | multiselect | autocomplete=producer,min=1 | Producer | 
|grantNumbers | management | N | Y |  | multiselect | autocomplete=grant,min=1 | Grant Information | 
|kindOfData | descriptors | Y | Y |  | checkbox | open=0 | Kind of Data | Audiovisual,Collection,Dataset,Event,Image,Interactive Resource,Model,Physical Object,Service,Software,Sound,Text,Workflow,Other
|keywords | descriptors | N | Y |  | multiselect | autocomplete=bioportal,onto=EFO:JERM:EDAM:MS:NMR:NCIT:OBI:PO:PTO:AGRO:ECOCORE:IOBC:NCBITAXON | Keywords | 
|topics | descriptors | N | Y |  | multiselect | autocomplete=VOvocab | Topic Classification | 
|dataOrigin | descriptors | N | Y |  | checkbox | open=0 | Data origin | observational data,experimental data,survey data,analysis data,text corpus,simulation data,aggregate data,audiovisual corpus,computer code,Other
|experimentfactor | descriptors | N | Y |  | multiselect | autocomplete=vocabulary,min=1 | Experimental Factor | 
|measurement | descriptors | N | Y |  | multiselect | autocomplete=vocabulary,min=1 | Measurement type | 
|technology | descriptors | N | Y |  | multiselect | autocomplete=vocabulary,min=1 | Technology type | 
|publication_citation | descriptors | N | Y |  | areabox | rows=5,cols=30 | Publication - Citation | 
|publication_idtype | descriptors | N | Y |  | dropbox | width=200px | Publication - ID Type | -,ark,arXiv,bibcode,doi,ean13,eissn,handle,isbn,issn,istc,lissn,lsid,pmid,purl,upc,url,urn
|publication_idnumber | descriptors | N | Y |  | textbox | width=400px | Publication - ID Number | 
|publication_url | descriptors | N | Y |  | textbox |  | Publication - URL | 
|comment | other | N | Y |  | areabox | rows=15, cols=30 | Additional information | 
