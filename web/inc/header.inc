<?php

$PATH='./';
$FUNC='';


if ($_SERVER['PATH_INFO']) {
	$urloptions = explode("/",$_SERVER['PATH_INFO']);
	array_shift($urloptions);
	if (count($urloptions)>0) {
		$FUNC =  array_shift($urloptions);
		$PATH='../';
	}
}

$TITLE = $MAINTITLE;

#------------------------

$HEAD1 = '
	<meta http-equiv="Content-Type" content="text/html;charset=utf-8" />
	<link rel="icon" href="'.$PATH.'favicon.ico" />
	<!-- Font -->
	<link rel="preconnect" href="https://fonts.googleapis.com">
	<link rel="preconnect" href="https://fonts.gstatic.com" crossorigin>
	<link href="https://fonts.googleapis.com/css2?family=Roboto+Flex:wght@400&display=swap" rel="stylesheet">
';

$HEAD2 = '
	<!-- CSS Bootstrap -->
	<link rel="stylesheet" href="https://cdn.jsdelivr.net/npm/bootstrap@5.0.2/dist/css/bootstrap.min.css">
	<!-- CSS Bootstrap Font Icon  -->
	<link rel="stylesheet" href="https://cdn.jsdelivr.net/npm/bootstrap-icons@1.10.5/font/bootstrap-icons.css">
';

$HEAD3 = '
	<!-- Bootstrap JS Bundle -->
	<script src="https://cdn.jsdelivr.net/npm/bootstrap@5.0.2/dist/js/bootstrap.bundle.min.js"></script>
';

#---

$HEAD = $HEAD1.$HEAD2.'
	<!-- CSS Maggot -->
	<link rel="stylesheet" href="'.$PATH.'css/style.css">
	<!-- jQuery -->
	<script src="'.$PATH.'js/jquery/jquery.min.js"></script>
	<script src="'.$PATH.'js/formutils.js"></script>
'.$HEAD3;

#---

$HEADMAIN = $HEAD1.'
	<!-- CSS jQuery UI -->
	<!--link rel="stylesheet" href="'.$PATH.'css/jquery/jquery-ui.min.css"-->
	<link rel="stylesheet" href="https://code.jquery.com/ui/1.13.2/themes/base/jquery-ui.min.css">
'.$HEAD2.'
	<!-- CSS Maggot -->
	<link rel="stylesheet" href="'.$PATH.'css/jquery.autocomplete.css">
	<link rel="stylesheet" href="'.$PATH.'css/style.css">
	<link rel="stylesheet" href="'.$PATH.'css/ror-widget.css">
	<!-- jQuery -->
	<!--script src="'.$PATH.'js/jquery/jquery.min.js"></script-->
	<script src="https://code.jquery.com/jquery-3.6.0.min.js"></script>
	<!-- jQuery UI -->
	<!--script src="'.$PATH.'js/jquery/jquery-ui.min.js"></script-->
	<script src="https://code.jquery.com/ui/1.13.2/jquery-ui.min.js"></script>
'.$HEAD3.'
	<!-- JS Maggot -->
	<script src="'.$PATH.'js/bpsearch.min.js"></script>
	<script src="'.$PATH.'js/formutils.js"></script>
	<script src="'.$PATH.'js/bioportal.js"></script>
	<script src="'.$PATH.'js/skosmos.js"></script>
	<script src="'.$PATH.'js/typeahead.min.js"></script>
';

#------------------------

$header_open='<div class="entete"><table width="100%"><tr>
<td align="left"><img src="'.$PATH.'img/maggot-icon.png" height="32" width="35">&nbsp;<a class="entete" href="'.$PATH.'">'.$TITLE.'</a></td>
<td>&nbsp;</td>
<td align="right">';

$header_close='</td></tr></table></div>';

$header_title = $header_open.$header_close;

#------------------------

if( ! isset( $_GET['extern'] ) && ! $EXTERN )
$menu1 ='<div class="btn-group btn-group-lg align-to-right" role="group">
	<i class="bi bi-filetype-json" style="color:grey;"></i><a href="./entry" class="search_data"><button type="button" class="btn btn-primary" data-toggle="button" aria-pressed="false" autocomplete="off" name="btn-input">Metadata input</button></a>&nbsp;&nbsp;&nbsp;
	<i class="bi bi-search" style="color:grey;"></i><a href="./search" class="search_data"><button type="button" class="btn btn-primary" data-toggle="button" aria-pressed="false" autocomplete="off" name="btn-search">Dataset research</button></a>&nbsp;&nbsp;&nbsp;
	<i class="bi bi-question-octagon-fill" style="color:grey;"></i><a href="./doc/about" class="search_data" target="_blank"><button type="button" class="btn btn-primary" data-toggle="button" aria-pressed="false" autocomplete="off">About</button></a>
	</div>';

if( isset( $_GET['extern'] ) || $EXTERN )
$menu1 ='<div class="btn-group btn-group-lg" role="group">
	<a href="./dataverse" class="search_data"><button type="button" class="btn btn-primary" data-toggle="button" aria-pressed="false" autocomplete="off" name="btn-search">Publish Dataset</button></a>
	</div>';

$header_main = $header_open.$menu1.$header_close;

#------------------------

$header_doc = $header_open.'<div class="btn-group btn-group-lg" role="group">
	<i class="bi bi-filter" style="color:grey;"><a href="'.$PATH.'doc/meta" class="search_data"><button type="button" class="btn btn-primary" data-toggle="button" aria-pressed="false" autocomplete="off" name="btn-doc">Metadata</button></a><i class="bi bi-eye" style="color:grey;"><a href="'.$PATH.'doc/about" class="search_data"><button type="button" class="btn btn-primary" data-toggle="button" aria-pressed="false" autocomplete="off" name="btn-about">About</button></a>'.$header_close;

#------------------------

$header_config = $header_open.'<div class="btn-group btn-group-lg" role="group">
	<i class="bi bi-eye" style="color:grey;"><a href="'.$PATH.'config/view" class="search_data"><button type="button" class="btn btn-primary" data-toggle="button" aria-pressed="false" autocomplete="off" name="btn-conv2">View all TSV</button></a>
	<i class="bi bi-filetype-json" style="color:grey;"><a href="'.$PATH.'config" class="search_data"><button type="button" class="btn btn-primary" data-toggle="button" aria-pressed="false" autocomplete="off" name="btn-conv1">TSV to JSON</button></a>
	<i class="bi bi-filetype-md" style="color:grey;"><a href="'.$PATH.'config/doc" class="search_data"><button type="button" class="btn btn-primary" data-toggle="button" aria-pressed="false" autocomplete="off" name="btn-conv3">TSV to DOC</button></a>
	<i class="bi bi-question-octagon-fill" style="color:grey;"></i><a href="'.$maggot_doc_url.'configuration/" class="search_data" target="_blank"><button type="button" class="btn btn-primary" data-toggle="button" aria-pressed="false" autocomplete="off">Documentation</button></a>
	</div>'.$header_close;

#------------------------

$header_meta_1 = '
	<div class="form-group2" id="exportcont" style="display: none"><i class="bi bi-hand-index-thumb" style="color:grey;"></i><span id="export"></span></div>&nbsp;&nbsp;';

$header_meta_2 = '
	<div class="form-group2" id="filebrowsercont" style="display: none"><i class="bi bi-folder" style="color:grey;"></i><span id="filebrowser"></span></div>';

if ($export_dataverse || $export_zenodo || $export_bloxberg)
	$header_meta_1 = '
		<div class="form-group2" id="exportcont" style="display: none"><i class="bi bi-hand-index-thumb" style="color:grey;"></i>
		<select style="text-align-last:center; border:0; background: #edf1f6;" id="publish_sel" name="publish_sel" class="form-control-sm" onchange="gotoURL(this.value)">
		<option value="none">Publish the datasets</option></select></div>&nbsp;&nbsp;';

$header_meta = $header_open.$header_meta_1.$header_meta_2.$header_close;



?>