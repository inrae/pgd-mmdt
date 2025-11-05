<?php

$id_metadata=$FUNC;

// id given as a query parameter (GET)
if( isset( $_GET['id'] ) ){
	$id_metadata =  $_GET['id'];
}

if (isset($_GET['format']) && strlen($id_metadata)) {
	$error1 = '{ "error" : "file system error" }';
	$error2 = '{ "error" : "dataset "'.$id_metadata.'" seems unavailable" }';
	$error3 = '{ "error" : "Unknown format '.$_GET['format'].' or feature not enabled" }';
	$array_values = getMetadata ($id_metadata);
	$json = $error2;
	if (is_array($array_values)) {
		$path = str_replace('pgd_data', 'mnt', $array_values['chemin']);
		do {
			$json = $error1;
			if (! is_file($path)) break;
			$json = $error2;
			if (is_accessible ($array_values)) break;
			$input_json = file_get_contents($path);
			$json = $error3;
			if ($_GET['format']==='maggot') {
				$json = $input_json;
				break;
			}
			if ($export_dataverse && $_GET['format']==='dataverse') {
				$output_json='';
				$msg = maggot2dataverse ($input_json, $output_json);
				$json = strlen($msg) ? $msg : $output_json;
				break;
			}
			if ($export_zenodo && $_GET['format']==='zenodo') {
				$output_json='';
				$msg = maggot2zenodo ($input_json, $output_json);
				$json = strlen($msg) ? $msg : $output_json;
				break;
			}
			if ($export_jsonld && $_GET['format']==='jsonld') {
				$output_json='';
				$msg = maggot2schema ($input_json, $output_json);
				$json = strlen($msg) ? $msg : $output_json;
				break;
			} 
			if ($export_jsonld && $_GET['format']==='graph') {
				include ('inc/schema/graph.inc');
				exit();
			}
			if ($export_jsonld && $_GET['format']==='rdf') {
				$jsonld='';
				$msg = maggot2schema ($input_json, $jsonld);
				if (! strlen($msg)) {
					include ('inc/schema/easyRDF.inc');
					exit();
				} else {
					$json = $msg;
					break;
				}
			}
		} while(0);
	}

	echo header('content-type:application/json');
	echo $json;
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
	global $txtboite, $listboite, $checkboite, $multiboite, $dateboite, $areaboite;

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
	$obj = dbQueryToArray($filter, $options);
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
		if (isset($array_values[$field]) && is_array($array_values[$field])) {
			$arr_val=array();
			foreach( $array_values[$field] as $val ) array_push($arr_val, metadata_render($val));
			$value = display_array($arr_val);
		} else {
			$value='-';
			if (isset($array_values[$field])) $value = display_array(metadata_render($array_values[$field]));
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
	<?= $HEAD ?>

	<script type="text/javascript">
	async function export_file(name, format) {
		$('#waiting').css('display','block');
		if (format==='rdf')
			await fetch('<?= $PATH ?>metadata/'+name+'?format=rdf')
				.then(response => response.text())
				.then(data => {
					const blob = new Blob([data], { type: 'application/rdf+xml' });
					const url = URL.createObjectURL(blob);
					const link = document.createElement('a');
					link.href = url;
					link.download = name+'-rdf.xml';
					link.click();
					URL.revokeObjectURL(url);
					$('#export_sel').val('0');
					$('#waiting').css('display','none');
			});
		else
			await $.getJSON('<?= $PATH ?>metadata/'+name+'?format='+format, function (json) {
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
			path = '<?= $PATH ?>';
			id_metadata='<?= $id_metadata ?>';
			url = path+param+'?dataset='+id_metadata
			window.open(url, '_self');
		}
		const $select = document.querySelector('#publish_sel');
		$select.value = 'none'
	}
	function gotoURL2(url) {
		if (url !== 'none') {
			if(url.match(/entry.dataset=/) !== null)
				window.open(url, '_self');
			else
				window.open(url, '_blank');
		}
		const $select = document.querySelector('#other_sel');
		$select.value = 'none'
	}
	</script>
</head>

<body style="zoom: <?= $ZOOMWP ?>">

<?= $header_meta ?>
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

	// Check if the dataset is accessible
	if (is_accessible ($array_values)) {
		echo  '<div class="p-3 mb-2 bg-warning text-black">No metadata available for this dataset ...</div>';
		break;
	}

	// Title
	$title="Title";
	if (array_key_exists('title',$array_values))
		$title=$array_values['title'];

	// Title + Menus
	$my_html = '<table style="width: 100%;"><tr><td><h2 style="font-size: 24px;"><i class="bi bi-list" style="color:grey; font-size: 20px"></i>&nbsp;'.$title.'</h2></td><td align="right" style="width: 600px;"><div class="btn-group btn-group-lg" role="group">';

	$html_sel1='<select style="text-align-last:center; min-width: 200px;" id="export_sel" name="export_sel" class="form-control form-control-sm menu_select export_json" onchange="export_file(\''.$id_metadata.'\',$(this).val());"><option value="0" selected disabled hidden>Metadata export</option><option value="maggot">Maggot JSON Format</option>';
	$html_sel2='<select style="text-align-last:center; min-width: 200px;" id="other_sel" name="other_sel" class="form-control form-control-sm menu_select export_json" onchange="gotoURL2(this.value);"><option value="none" selected disabled hidden>Other Tools</option><option value="'.$PATH.'entry?dataset='.$id_metadata.'">Edit Metadata</option>';
	$html_sel3='<select style="text-align-last:center; min-width: 200px;" id="publish_sel" name="publish_sel" class="form-control form-control-sm menu_select export_json" onchange="gotoURL(this.value);"><option value="none" selected disabled hidden>Publish the datasets</option>';

	if ($export_dataverse && is_file($dataverse_conf) && is_file($mapping_conf)) {
		$html_sel3 .= '<option value="dataverse">Dataverse repository</option>';
		$html_sel1 .= '<option value="dataverse">Dataverse JSON Format</option>';
	}

	if ($export_zenodo && is_file($zenodo_conf)) {
		$html_sel3 .= '<option value="zenodo">Zenodo repository</option>';
		$html_sel1 .= '<option value="zenodo">Zenodo JSON Format</option>';
	}

	if ($export_jsonld)
		$html_sel1 .= '<option value="jsonld">JSON-LD Format</option><option value="rdf">RDF/XML Format</option>';

	if ($export_bloxberg)
		$html_sel3 .= '<option value="bloxberg">bloxberg blockchain</option>';

	if ($FILEBROWSER)
		$html_sel2 .= '<option value="'.$URL_FILEBROWSER.'">File Browser</option>';

	if ($export_jsonld)
		$html_sel2 .= '<option value="'.$id_metadata.'?format=graph">View RDF Graph</option>';

	$my_html .=  $html_sel1.'</select>';
	if ($export_dataverse || $export_zenodo || $export_bloxberg)
		$my_html .=  '&nbsp;&nbsp;'.$html_sel3.'</select>';
	$my_html .= '&nbsp;&nbsp;'.$html_sel2.'</select></div><div id="waiting" style="display: none; max-width:1000px; margin:auto;"><img src="'.$PATH.'img/throbber.gif" width="100px"></div></td></tr></table><br>';
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
				if (isset($fields[$j]) && $fields[$j]==='title') { $jmax++; continue; }
				if (isset($fields[$j]) && $fields[$j]==='description') { $jmax++; continue; }
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
<br><br><br>
