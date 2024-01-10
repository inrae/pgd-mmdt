<?php

$id_metadata=$FUNC;

// id given as a query parameter (GET)
if( isset( $_GET['id'] ) ){
	$id_metadata =  $_GET['id'];
}

if (isset($_GET['format']) && strlen($id_metadata)) {
	$array_values = getMetadata ($id_metadata);
	if (is_array($array_values)) {
		$path = str_replace('pgd_data', 'mnt', $array_values['chemin']);
		do {
			if (! is_file($path)) break;
			if (is_private ($array_values)) break;
			$json = file_get_contents($path);
			if ($export_dataverse && $_GET['format']==='dataverse') {
				$output_json='';
				$msg = maggot2dataverse ($json, $output_json);
				$json = $output_json;
				break;
			}
			if ($export_jsonld && $_GET['format']==='jsonld') {
				$output_json='';
				$msg = maggot2schema ($json, $output_json);
				$json = $output_json;
				break;
			}
			if ($export_zenodo && $_GET['format']==='zenodo') {
				$output_json='';
				$msg = maggot2zenodo ($json, $output_json);
				$json = $output_json;
				break;
			}
			if (! $LOG) header('content-type:application/json');
			echo header('content-type:application/json');
			echo $json;
		} while(0);
	}
	exit();
}


if (isset($_GET['col'])) {
	$NBCOLS=min(10,max(3,intval($_GET['col'])));
}
if (isset($_GET['media'])) {
	$MEDIA = intval($_GET['media']);
}

if (isset($_GET['empty'])) {
	$EMPTY = intval($_GET['empty']);
}

function getMetadata ($id_metadata) {
	global $client, $txtboite, $listboite, $checkboite, $multiboite, $dateboite, $areaboite;

	// Options
	$options = array('projection'=>[]);
	foreach(array_keys((array)$txtboite) as $key) $options['projection'][$key]=true;
	foreach(array_keys((array)$listboite) as $key) $options['projection'][$key]=true;
	foreach(array_keys((array)$checkboite) as $key) $options['projection'][$key]=true;
	foreach(array_keys((array)$multiboite) as $key) $options['projection'][$key]=true;
	foreach(array_keys((array)$dateboite) as $key) $options['projection'][$key]=true;
	foreach(array_keys((array)$areaboite) as $key) $options['projection'][$key]=true;
	$options['projection']['chemin']=true;

	// Requête pour récupérer l'ensemble des champs de la table metadata pour ce jeu de donnée
	$filter = [ 'title' => $id_metadata ];
	$query = new MongoDB\Driver\Query($filter, $options);
	$obj = $client->executeQuery('pgd-db.metadata', $query)->toArray();
	$ret = count($obj);
	if ($ret > 0) {
		$ret = (array)($obj[0]);
	} else {
		$ret=0;
	}
	return $ret;
}

function build_table($section, $arr) {
	global $array_values, $EMPTY;

	$tblhead =''; $tblcore='';
	foreach($arr as $field) {
		if ($field==='title' || $field==='description') continue;
		if (is_array($array_values[$field])) {
			$arr_val=array();
			foreach( $array_values[$field] as $val ) array_push($arr_val, metadata_render($val));
			$value = display_array($arr_val);
		} else {
			$value = display_array(metadata_render($array_values[$field]));
		}
		if ($EMPTY>0 || $value !== '-') {
			$tblhead .='<th scope="col">'.get_label($field).'</th>';
			$tblcore .='<td>'.$value.'</td>';
		}
	}
	$myhtml='<table class="table"><thead class="thead-light"><tr>'.$tblhead.'</tr></thead><tbody><tr>'.$tblcore.'</tr></tbody></table>';
	return $myhtml;
}

?>

<!DOCTYPE html>
<html lang="en" >

<head>
	<title>Maggot - Metadata</title>
	<?php echo $HEAD ?>

	<script type="text/javascript">
	function export_json(name, format) {
		$.ajaxSetup({async:true});
		$('#waiting').css('display','block');
		$.getJSON('<?php echo $PATH ?>metadata/'+name+'?format='+format, function (json) {
			const bytes = new TextEncoder().encode(JSON.stringify(json));
			const blob = new Blob([bytes], { type: "application/json;charset=utf-8" });
			const url = window.URL.createObjectURL(blob);
			const a = document.createElement('a');
			a.style.display = 'none';
			a.href = url;
			a.download = name+'_'+format+'.json';
			document.body.appendChild(a);
			a.click();
			window.URL.revokeObjectURL(url);
			$('#export_sel').val('0');
			$('#waiting').css('display','none');
		});
	}
	function gotoURL(param) {
		if (param !== 'none') {
			path = '<?php echo $PATH ?>';
			id_metadata='<?php echo $id_metadata ?>';
			url = path+param+'?dataset='+id_metadata
			window.open(url, '_self');
		}
	}
	</script>
</head>

<body style="zoom: <? echo $ZOOMWP ?>">

<?php echo $header_meta ?>
<br>

<div style="padding-bottom: 10px;"><div id="errbox" name="errbox" class="msgbox errbox">
	<div style="margin-left: 2rem; margin-top: 1rem; margin-bottom: 0rem;">
		<span id="errboxmsg">Error !!!</span></p>
	</div>
</div></div>

<div>

<?php

do {
	if( ! strlen($id_metadata) ) {
		$msg='Error: the dataset identifier is missing !!!';
		echo "<script>$('#errboxmsg').html('".$msg."'); $('#errbox').css('display','block');</script>";
		break;
	}

	// Get all values
	$array_values = getMetadata ($id_metadata);
	if (! is_array($array_values)) {
		echo  '<div class="p-3 mb-2 bg-warning text-black">No metadata available for this dataset ...</div>';
		break;
	}

	// If Private access file exits
	if (is_private ($array_values)) {
		echo  '<div class="p-3 mb-2 bg-warning text-black">No metadata available for this dataset ...</div>';
		break;
	}

	// Title
	$title="Title";
	if (array_key_exists('title',$array_values))
		$title=$array_values['title'];

	// Title + Export JSON
	$my_html = '<table style="width: 100%;"><tr><td><h2 style="font-size: 24px;"><i class="bi bi-list" style="color:grey; font-size: 20px"></i>&nbsp;'.$title.'</h2></td><td align="right" style="width: 400px;"><div class="btn-group btn-group-lg" role="group"><select style="text-align-last:center;" id="export_sel" name="export_sel" class="form-control form-control-sm export_json" onchange="export_json(\''.$id_metadata.'\',$(this).val());"><option value="0">--- Metadata export ---</option><option value="maggot">Maggot JSON Format</option>';

	if ($FILEBROWSER)
		echo "<script>$('#filebrowser').html('<a href=\"".$URL_FILEBROWSER."\" class=\"search_data\" target=\"_blank\"><button type=\"button\" class=\"btn btn-primary\" style=\"max-height: 32px;padding-top: 0.2rem;\" data-toggle=\"button\" aria-pressed=\"false\" autocomplete=\"off\" name=\"btn-search\">File Browser</button></a>');$('#filebrowsercont').css('display','inline-block')</script>";

	$dropselect = ($export_dataverse && $export_zenodo) ? 1 : 0;

	if ($export_dataverse && is_file($dataverse_conf) && is_file($mapping_conf)) {
		if ($dropselect)
			echo "<script>$('#publish_sel').append(`<option value=\"dataverse\">Dataverse repository</option>`); $('#exportcont').css('display','inline-block');</script>";
		else
			echo "<script>$('#export').html('<a href=\"".$PATH."dataverse?dataset=".$id_metadata."\" class=\"search_data\"><button type=\"button\" class=\"btn btn-primary\" style=\"max-height: 32px;padding-top: 0.2rem;\" data-toggle=\"button\" aria-pressed=\"false\" autocomplete=\"off\" name=\"btn-search\">Publish the Dataset</button></a>');$('#exportcont').css('display','inline-block');</script>";
		$my_html .= '<option value="dataverse">Dataverse JSON Format</option>';
	}

	if ($export_zenodo && is_file($zenodo_conf)) {
		if ($dropselect)
			echo "<script>$('#publish_sel').append(`<option value=\"zenodo\">Zenodo repository</option>`); $('#exportcont').css('display','inline-block');</script>";
		else
			echo "<script>$('#export').html('<a href=\"".$PATH."zenodo?dataset=".$id_metadata."\" class=\"search_data\"><button type=\"button\" class=\"btn btn-primary\" style=\"max-height: 32px;padding-top: 0.2rem;\" data-toggle=\"button\" aria-pressed=\"false\" autocomplete=\"off\" name=\"btn-search\">Publish the Dataset</button></a>');$('#exportcont').css('display','inline-block');</script>";
		$my_html .= '<option value="zenodo">Zenodo JSON Format</option>';
	}

	if ($export_jsonld)
		$my_html .= '<option value="jsonld">JSON-LD Format</option>';

	$my_html .=  '</select></div><div id="waiting" style="display: none; max-width:1000px; margin:auto;"><img src="'.$PATH.'img/throbber.gif" width="100px"></div></td></tr></table><br>';
	echo $my_html;

	// Description of the Dataset
	if (array_key_exists('description',$array_values)) {
		echo '<h3>DESCRIPTION</h3>';
		$newval = preg_replace('"(https?://[^ ]+)"', '<a href="$1" target="_blank">$1</a>', str_replace( ['_NL_'], ['<br>'], $array_values['description']));
		echo "<h2 class='description'>".$newval."</h2><br/>";
	}

	//For each section
	foreach($section['labels'] as $name => $text) {
		if ($name==="resources") continue;
		$myhtml='<h3>'.$text.'</h3><div class="table-responsive-sm">';
		$fields = array_keys(array_flip($section[$name]));
		# Display of fields in groups of $NBCOLS on the same line
		for($i=0; $i<count($fields); $i+=$NBCOLS) {
			$arr=[]; $jmax=$i+$NBCOLS;
			for($j=$i; $j<$jmax; $j++) {
				if ($fields[$j]==='title') { $jmax++; continue; }
				if ($fields[$j]==='description') { $jmax++; continue; }
				if ($j<count($fields)) { array_push($arr, $fields[$j]); }
			}
			$myhtml=$myhtml.build_table($name, $arr);
		}
		$myhtml=$myhtml.'</div><br/>';
		if (preg_match('/scope/', $myhtml))
			echo $myhtml;
	}

	// Check if Resources
	$path=$array_values['chemin'];
	$json_file = str_replace('pgd_data','mnt',$path);
	$json_content = json_decode(file_get_contents($json_file), NULL, 512, JSON_OBJECT_AS_ARRAY);
	if (! array_key_exists('resources',$json_content)) continue;
	$res = $json_content['resources'];
	if (count($res)<1) continue;

	// Resources
	echo '<h3>RESOURCES</h3>';
	$myhtml='<table class="table"><thead class="thead-light">';
	$headmedia='';
	$bpurl='https://bioportal.bioontology.org/ontologies/FDC-GDMT/?p=classes&conceptid=http%3A%2F%2Fvocab.fairdatacollective.org%2Fgdmt%2F';
	if ($MEDIA)
		$headmedia='<th scope="col">Media</th>';
	$myhtml=$myhtml.'<tr><th scope="col">Type</th>'.$headmedia.'<th scope="col">Description</th><th scope="col">Location</th></tr></thead><tbody>';
	for($i=0; $i<count($res); $i++) {
		$htmlmedia='';
		if ($MEDIA) $htmlmedia = strlen($res[$i]['media']) ?
			'<td><a target="_blank" href="'.$bpurl.str_replace('/','-',$res[$i]['media']).'">'.$res[$i]['media'].'</a></td>' :
			'<td></td>';
		$location=preg_replace('"(https?://[^ ]+)"', '<a href="$1" target="_blank">$1</a>',$res[$i]['location']);
		$myhtml=$myhtml.'<tr><td>'.$res[$i]['datatype'].'</td>'.$htmlmedia.'<td>'.$res[$i]['description'].'</td><td>'.$location.'</td></tr>';
	}
	$myhtml=$myhtml.'</tbody></table><br/>';
	echo $myhtml;

} while(0);

?>

</div>
<br>
