<?php

# Nom du fichier: home_left.inc
# Auteur(s): D.JACOB 
# Copyright: (c) INRAE - 2015-2025
#

# Banner
$BANNER="An ecosystem for producing and sharing metadata";

# Left Panel
$links[0]="https://www.ouvrirlascience.fr/dmp-opidor-outil-pour-faciliter-la-redaction-de-plans-de-gestion-de-donnees/";
$links[1]="https://www.ouvrirlascience.fr/deuxieme-plan-national-pour-la-science-ouverte/";
$links[2]="https://recherche.data.gouv.fr/";
$links[3]="https://www.ouvrirlascience.fr/fair-principles/";
$links[4]="https://wp.u-picardie.fr/open-science/wp-content/uploads/sites/22/2021/07/Fiche-Synthetique-Metadonnees.pdf";
$links[5]="https://inrae.github.io/pgd-mmdt/tutorial/";

$HTMLTEXT = '
The <b>Maggot</b> tool allows users to describe the <a href="'.$links[4].'" target=\"_blank">Metadata</a> of their datasets produced within a collective (research unit, platform, multi-partner project, etc.), thus making it possible
<ul>
	<li>to answer certain questions of the <b>Data Management Plan</b> (<a href="'.$links[0].'" target="_blank">DMP</a>) concerning the <b>organization, documentation, storage and sharing of data</b> in the data storage space,</li>
	<li>to meet certain data and metadata requirements, referred to by the <a href="'.$links[1].'" target="_blank">National Plan for Open Science</a> , in particular its second axis: “<b>Structure, Share and Open Research Data</b>” (<a href="'.$links[2].'" target="_blank">Recherche Data Gouv</a>) in accordance with the <a href="'.$links[3].'" target="_blank">FAIR principles</a>.</li>
</ul>
See <a href="'.$links[5].'" target="_blank">Quick tutorial</a>.
';

?>