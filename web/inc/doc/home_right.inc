<?php

# Nom du fichier: home_right.inc
# Auteur(s): D.JACOB 
# Copyright: (c) INRAE - 2015-2025
#

# Right Panel
$OPTIONS = [
	'option1' => ['active' => $CONFIGURATION, 'row' => 1,
		'label' => 'Configuration', 'url' => 'viewconf', 'icon' => 'gear', 
		'info' => '<b>Configure</b> the terminology definition file'],
	'option2' => ['active' => $PRIVATE_ACCESS, 'row' => 1,
		'label' => 'Private access keys', 'url' => 'access', 'icon' => 'lock-fill',
		'info' => '<b>Manage</b> private access keys'],
	'option3' => ['active' => 1, 'row' => 2,
		'label' => 'Dictionaries', 'url' => 'dictionary', 'icon' => 'person',
		'info' => '<b>Edit</b> the dictionaries (need admin access to edit or just use "maggot" user for viewing only)'],
	'option4' => ['active' => 1, 'row' => 3,
		'label' => 'Generate metadata', 'url' => 'entry', 'icon' => 'filetype-json',
		'info' => '<b>Describe</b> a dataset using metadata of different types by facilitating their entry and avoiding their re-entry thanks to personalized dictionaries, then generate the metadata file'],
	'option5' => ['active' => 1, 'row' => 3,
		'label' => 'Search datasets', 'url' => 'search', 'icon' => 'search',
		'info' => '<b>Search</b> datasets based on some metadata - assuming the metadata were previously deposited in the storage space'],
	'option6' => ['active' => 0, 'row' => 1, # $FILEBROWSER
		'label' => 'File Browser', 'url' => '/fb/', 'icon' => 'card-list',
		'info' => '<b>File Browser</b> allows you to browse data directories using cloud-like software'],
	'option7' => ['active' => $export_dataverse, 'row' => 4,
		'label' => 'Push to Dataverse', 'url' => 'dataverse', 'icon' => 'hand-index',
		'info' => '<b>Publish</b> the metadata of datasets along with their data files into the <b>Recherche Data Gouv</b> repository'],
	'option8' => ['active' => $export_zenodo, 'row' => 4,
		'label' => 'Push to Zenodo', 'url' => 'zenodo', 'icon' => 'hand-index',
		'info' => '<b>Publish</b> the metadata of datasets along with their data files into the <b>Zenodo</b> repository'],
	'option9' => ['active' => $DEV ? $export_bloxberg : 0, 'row' => 4, # $export_bloxberg
		'label' => 'Push to Bloxberg', 'url' => 'bloxberg', 'icon' => 'hand-index',
		'info' => '<b>Publish</b> the metadata of datasets into the <b>bloxberg blockchain</b> (<i>Experimental</i>)'],
	'option10' => ['active' => 0, 'row' => 6,
		'label' => 'Import from DMP Opidor', 'url' => '', 'icon' => 'box-arrow-in-up-right',
		'info' => ''],
	'option11' => ['active' => 0, 'row' => 6,
		'label' => 'Import from eLabFTW', 'url' => '', 'icon' => 'box-arrow-in-up-right',
		'info' => '']
];

# Separator after rows
$ROW_SEPARATOR = [2];

# Group options by line
$OPTGROUPS = [1 => [], 2 => [], 3 => [], 4 => [], 5 => [], 6 => []];
foreach ($OPTIONS as $opt) {
    if ($opt['active'] && isset($opt['row'])) {
        $OPTGROUPS[$opt['row']][] = $opt;
    }
}

?>