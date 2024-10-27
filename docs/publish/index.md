---
title: Maggot - Publish Metadata
summary: 
authors:
    - Daniel Jacob
date: 2023-10-09
some_url:
---

# Publish Metadata

<style>.md-typeset h1 {display: none;} .md-nav__item {font-size: medium}</style>

###  Publish Metadata

* Once we have decided to publish our metadata with possibly our data, we can choose the repository that suits us. Currently repositories based on [Dataverse][4]{:target="_blank"} and [Zenodo][5]{:target="_blank"} are supported, both being [Europe-approved repositories][6]{:target="_blank"}.
* Using an approach that might be called “[machine-readable][2]{:target="_blank"} metadata,” it is possible to populate metadata for a dataset into one of the proposed data repositories via its [web API][3]{:target="_blank"}, provided that you have taken care to correctly define your metadata schema so that it is possible to make a correspondence with the chosen data repository using a mapping definition file.

* The principle is illustrated by the figure above.

<center>
<a href="../images/publish_fig1.png" data-lightbox="fig1"><img src="../images/publish_fig1.png" width="800px"></a>
</center>


* We start from the Maggot JSON format metadata file generated from the web interface and based on the metadata profile defined by the terminology definition files. 
* Then from a file defining the correspondence between the Maggot fields and those of the target repository, we can perform a [metadata crosswalk](../chats/chat4){:target="_blank"} to the JSON format supported by the web API of the target repository.
* During the process we enrich the metadata with controlled vocabularies based either on dictionaries or on thesauri and/or ontologies. For the latter cases, we use the web APIs of these sources to perform the mapping (see the [definition of mapping](../definitions/mapping)).
* Finally, to be able to carry out the transfer i.e. the submission to the target repository (we say "push" for short), we first need to connect to the repository in order to retrieve the key (the [API token][1]{:target="_blank"}) authorizing us to submit the dataset. This obviously assumes that we have the privileges (creation/modification rights) to do so.


[1]: https://www.wallarm.com/what/what-is-an-api-token-quick-guide
[2]: https://opendatahandbook.org/glossary/en/terms/machine-readable/
[3]: https://en.wikipedia.org/wiki/Web_API
[4]: https://dataverse.org/
[5]: https://www.openaire.eu/zenodo-guide
[6]: https://open-research-europe.ec.europa.eu/for-authors/data-guidelines#approvedrepositories

# https://www.google.com/search?q=metadata+crosswalk+definition&oq=metadata+crosswalk