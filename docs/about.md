---
title: Maggot - About
summary: 
authors:
    - Daniel Jacob
date: 2023-10-09
some_url:
---

# About

<style>.md-typeset h1 {display: none;} .md-nav__item {font-size: medium}</style>

### Maggot, a research data cataloging tool 

#### Motives

* Meet the challenges of organizing, documenting, storing and sharing data within a collective (unit, platform, project, etc.).
* Have visibility of what is produced within the collective: datasets, software, images, sounds, videos, analyses, codes, etc.
* Fall within an open science approach for sharing and reproducibility by promoting FAIR principles within the collective.
* Raise awareness among newcomers and students about a better description of what they produce.

#### State of need

* Implementing a data management plan imposes prerequisites such as the externalization of data to be preserved outside of users' disk space. This does not only concern published data but all data produced during the duration of a project. Above all, this outsourcing makes it possible to gather the data in one place and already constitutes a first-level backup. This becomes even more necessary when temporary agents (doctoral students, post-docs, interns, fixed-term contracts) are involved in data production.
* Consequently, the concern arises about the organization of these storage spaces. Should they be harmonized, i.e. impose good practices such as _i_) the naming of folders and files, _ii_) a folder structure (docs, data, scripts, etc.), _iii_) the use of README files, etc.
* At a minimum, using a README file seems the simplest and least restrictive. But then the question arises “what to put in it”? Templates can be offered to simplify their writing. But then the question arises of how to use them effectively when we want to find information? With what vocabulary?

#### Proposed approach

* The two main ideas behind the tool are:
     * Make the data storage space a data repository without having to move the data, then ensure that the metadata gets to the data.
     * Be able to “capture” the user’s metadata as easily as possible by using their vocabulary.
* Concerning the first idea: "Just" place a metadata file (JSON format) describing the project data in each subdirectory, and then find the projects and/or data corresponding to specific criteria. The choice fell on the JSON format, very suitable for describing metadata, readable by both humans and machines.
* Concerning the second idea: Given the diversity of the fields, the approach chosen is to be both the most flexible and the most pragmatic possible by allowing users to choose their own vocabulary (controlled or not) corresponding to the reality of their field and their activities. However, a good approach is as much as possible to use only controlled vocabulary, that is to say relevant and sufficient vocabulary used as a reference in the field concerned to allow users to describe a project and its context without having to add additional terms. To this end, the tool must allow users a progressive approach towards the adoption of standardized controlled vocabularies (thesauri or even ontologies).

* With the approach proposed by Maggot, initially there is no question of opening the data, but of managing metadata associated with the data on a storage space with a precise perimeter represented by the collective (unit, team, project , platform, …). The main characteristic of the tool is, above all, to “capture” the metadata as easily as possible according to a well-chosen metadata schema. However, the opening of data via their metadata must be a clearly stated objective within the framework of projects financed by public institutions (e.g Europe). Therefore if you have taken care to correctly define your metadata schema so that it is possible to make a [metadata crosswalk](../chats/chat4){:target="_blank"} (using a mapping file) with a data repository recognized by the international community, then you can easily "push" its metadata with the data without having re-enter anything.

---

### Links

* **Source code on Github** : [inrae/pgd-mmdt][10]{:target="_blank"}
* **Issues tracker** : [inrae/pgd-mmdt/issues][16]{:target="_blank"}
* **Instance online** : [INRAE UMR 1322 BFP][11]{:target="_blank"}

### Publication

* Daniel Jacob, François Ehrenmann, Romain David, Joseph Tran, Cathleen Mirande-Ney, Philippe Chaumeil, An ecosystem for producing and sharing metadata within the web of FAIR Data, GigaScience, Volume 14, 2025, giae111, [DOI:10.1093/gigascience/giae111](https://academic.oup.com/gigascience/article/doi/10.1093/gigascience/giae111/7945442){:target="_blank"}

### Related articles

* Créez, gérez et partagez vos métadonnées avec Maggot (2025) - [VO-INRAE][22]{:target="_blank"}
* Usages d'AgroPortal dans des systèmes d'information à INRAE (2025) - [hal-05077024][21]{:target="_blank"}


### Contacts

* Daniel Jacob ([INRAE UR BIA-BIBS][2]{:target="_blank"} - [0000-0002-6687-7169][17]{:target="_blank"})


### Discussion space

* Issues tracker on [github][16]{:target="_blank"}
* [Tchap][18]{:target="_blank"} : Only for the French Education and Research (ESR) community


### Designers / Developers

* Daniel Jacob ([INRAE UR BIA-BIBS][2]{:target="_blank"})

* François Ehrenmann ([INRAE UMR BioGECO][1]{:target="_blank"})

* Philippe Chaumeil ([INRAE UMR BioGECO][1]{:target="_blank"})


### Contributors

* [INRAE](https://www.inrae.fr/en){:target="_blank"}
    * Edouard Guitton ([Dept. SA][6]{:target="_blank"}, [PFIE][3]{:target="_blank"}), David Benaben  & Cathleen Mirande-Ney ([UMR BFP][1]{:target="_blank"}), Joseph TRAN ([UMR EGFV][12]{:target="_blank"}), Stéphane Bernillon ([UR MycSA][14]{:target="_blank"})

* [CIRAD](https://www.cirad.fr/en){:target="_blank"}
    * Julien Paul ([AGAP institute][7]{:target="_blank"})

* [ERINHA][5]{:target="_blank"}
    * Romain David 

* [Mines de Paris - PSL][4]{:target="_blank"}
    * Amélie Masson, Sandrine Mouret, Sébastien Perrin



### License

* GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007 - [GPLv3][20]{:target="_blank"}


*[JSON]: JavaScript Object Notation : format used to represent structured information
*[autocompletion]: feature in which an application predicts the rest of a word a user is typing
*[FAIR]: Findable, Accessible, Interoperable and Reusable

[1]: https://www6.bordeaux-aquitaine.inrae.fr/biogeco_eng/
[2]: https://eng-ur-bia.angers-nantes.hub.inrae.fr/
[3]: https://eng-pfie.val-de-loire.hub.inrae.fr
[4]: https://www.minesparis.psl.eu/en/home/
[5]: https://erinha.eu/
[6]: https://www.inrae.fr/en/divisions/sa
[7]: https://umr-agap.cirad.fr/en
[8]: https://recherche.data.gouv.fr/en

[10]: https://github.com/inrae/pgd-mmdt
[11]: https://pmb-bordeaux.fr/maggot/

[12]: https://eng-egfv.bordeaux-aquitaine.hub.inrae.fr/

[14]: https://eng-mycsa.bordeaux-aquitaine.hub.inrae.fr/


[16]: https://github.com/inrae/pgd-mmdt/issues
[17]: https://orcid.org/0000-0002-6687-7169
[18]: https://www.tchap.gouv.fr/#/room/!fMpqgxVmYfzmotjVoQ:agent.agriculture.tchap.gouv.fr

[20]: https://www.gnu.org/licenses/gpl-3.0.html

[21]: https://hal.science/hal-05077024
[22]: https://vocabulaires-ouverts.inrae.fr/outil-maggot/

<br><br>

<center>
<a href="../images/LogoCloud.png" data-lightbox="fig1"><img src="../images/LogoCloud.png" width="800px"></a>
</center>

<br><br>