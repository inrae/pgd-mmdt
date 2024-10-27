---
title:  Maggot - Metadata File
summary: 
authors:
    - Daniel Jacob
date: 2023-10-09
some_url:
---

# Quick tutorial

<style>.md-typeset h1 {display: none;} .md-nav__item {font-size: medium}</style>

### Metadata File

Once the form has been completed, even partially (at least those which are mandatory and marked with a red star), you can export your metadata in the form of a file. The file is in JSON format and must have the prefix 'META_'.

By clicking on the "Generate the metadata file" button, you can save it on your disk space. 

<center>
<a href="../../images/tuto2_fig10.png" data-lightbox="fig10"><img src="../../images/tuto2_fig10.png" width="600px"></a>
</center>

Furthermore, if email sending has been configured (see [settings](../../settings/#incconfigconfiginc)), then you have the possibility to send the metadata file to the data managers for curation, and possibly also to support its storage on data disk space if specific rights are required.

<center>
<a href="../../images/tuto2_fig11.png" data-lightbox="fig11"><img src="../../images/tuto2_fig11.png" width="500px"></a>
</center>

In case you want to save the metadata file on your disk space, you have two ways to use this file:

__1.__ The first use is the recommended one because it allows metadata management within your collective.
: <p>You drop the metadata file directly under the data directory corresponding to the metadata. Indeed, when installing the tool, a storage space dedicated to the tool had to be provided for this purpose. See [infrastructure](../../infrastructure). Once deposited, you just have to wait around 30 minutes maximum so that the tool has had time to scan the root of the data directories looking for new files in order to update the database. After this period, the description of your dataset will be visible from the interface, and a selection of criteria will be made in order to restrict the search.</p>

<center>
<a href="../../images/tuto2_fig7.png" data-lightbox="fig7"><img src="../../images/tuto2_fig7.png" width="800px"></a>
</center>

<br>
: <p>You will then have the possibility to publish the metadata later with possibly the corresponding data in a data repository such as [Dataverse](../../publish/dataverse/) or [Zenodo](../../publish/zenodo/).</p>

__2.__ The second use is only to deposit the metadata into a data repository
: <p>Whether with [Dataverse](../../publish/dataverse/) or [Zenodo](../../publish/zenodo/), you have the possibility to publish metadata directly in one or other of these repositories without using the storage space.</p>

<center>
<a href="../../images/tuto2_fig8.png" data-lightbox="fig8"><img src="../../images/tuto2_fig8.png" width="800px"></a>
</center>

<br>
: <p>Please note that you cannot also deposit the data files in this way. You will have to do this manually for each of them directly online in the repository.</p>

<br><br><br>

*[TSV]: Open text format representing tabular data as "Tab-Separated Values". Each row corresponds to a table row and the cells in a row are separated by a tab.
*[JSON]: JavaScript Object Notation : format used to represent structured information.
*[autocomplete]: feature in which an application predicts the rest of a word a user is typing.
