---
title: Maggot - Definition Files
summary: 
authors:
    - Daniel Jacob
date: 2023-10-09
some_url:
---

# Definition Files

<style>.md-typeset h1 {display: none;} .md-nav__item {font-size: medium}</style>

### Metadata definition files

The Maggot tool offers [great flexibility in configuration][6]{:target="_blank"}. It allows you to completely choose all the metadata you want to describe your data. You can base yourself on an existing [metadata schema][1]{:target="_blank"}, invent [your own schema][2]{:target="_blank"} or, more pragmatically, mix one or more schemas by introducing some metadata specific to your field of application. However, keep in mind that if you want to add descriptive metadata to your data then a [certain amount of information][3]{:target="_blank"} is expected. But a completely different use of the tool is possible, it's up to you.

There are **two levels of definition files** as shown the figure below:

<center><a href="../images/definitions_fig2.png" data-lightbox="fig2"><img src="../images/definitions_fig2.png" width="600px"></a></center>

**1** - The first level concerns the definition of terminology (metadata) similar to a [descriptive metadata plan][7]{:target="_blank"}. Clearly, this category is more akin to configuration files. They represent the heart of the application around which everything else is based. The input and search interfaces are completely generated from these definition files (especially the *[web/conf/config_terms.txt](terminology)* file), thus defining each of the fields, their <a href="../images/terminology_types.png" data-lightbox="fig3">input type</a> (*checkbox*, *dropbox*, *textbox*, ...) and the associated [controlled vocabulary](vocabulary) (ontology and thesaurus by autocompletion, drop-down list according to a list of fixed terms). This is why a [configuration step](../configuration) is essential in order to be able to configure all the other modules.

**2** - The second level concerns the definitions of the mapping to a differently structured metadata schema ([metadata crosswalk](../chats/chat4){:target="_blank"}, *i.e* a specification for mapping one metadata standard to another), used either ***i)*** for metadata export to a remote repository (*e.g.* [Dataverse](dataverse), [Zenodo](zenodo)) or ***ii)*** for metadata harvesting (*e.g.* [JSON-LD](json-ld), [OAI-PMH](oai-pmh)). Simply place the definition files in the configuration directory (*[web/conf][4]{:target="_blank"}*) for them to be taken into account, provided you have adjusted the configuration (See [Settings](../settings/#incconfigconfiginc)).

All definition files are made using a simple spreadsheet then exported in TSV format. 

The list of definition files in Maggot are given below. All must be put under the directory *[web/conf][4]{:target="_blank"}*.
<center><a href="https://pmb-bordeaux.fr/maggot/config/view" target="_blank"><img src="../images/definitions_fig1.png" width="600px"></a></center>

See an example on line : [https://pmb-bordeaux.fr/maggot/config/view](https://pmb-bordeaux.fr/maggot/config/view){:target="_blank"} and the [corresponding form](https://pmb-bordeaux.fr/maggot/entry){:target="_blank"} based on these definition files.

<br><br>


*[TSV]: Open text format representing tabular data as "Tab-Separated Values". Each row corresponds to a table row and the cells in a row are separated by a tab
*[JSON]: JavaScript Object Notation : format used to represent structured information
*[autocompletion]: feature in which an application predicts the rest of a word a user is typing


[1]: https://en.wikipedia.org/wiki/Metadata_standard
[2]: https://committee.iso.org/files/live/sites/tc46sc11/files/documents/N800R1%20Where%20to%20start-advice%20on%20creating%20a%20metadata%20schema.pdf
[3]: https://libraries.mit.edu/data-management/store/documentation/
[4]: https://github.com/inrae/pgd-mmdt/tree/main/web/conf
[5]: https://github.com/inrae/pgd-mmdt/blob/main/web/inc/config/config.inc
[6]: https://inrae.github.io/pgd-mmdt/pdf/MAGGOT_OpenData_Oct2023.pdf?download=false
[7]: https://sustainableheritagenetwork.org/system/files/atoms/file/How_to_Create_a_Descriptive_Metadata_Plan.pdf

<!--
https://rd-alliance.github.io/Research-Metadata-Schemas-WG/
https://www.rd-alliance.org/group/research-metadata-schemas-wg/outcomes/collection-crosswalks-fifteen-research-data-schemas
https://www.getty.edu/research/publications/electronic_publications/intrometadata/crosswalks.html
-->