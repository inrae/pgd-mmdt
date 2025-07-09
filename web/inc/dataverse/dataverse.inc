<?php
#
# Dataverse implementation 
#
# Auteur(s): D.JACOB 
# Copyright: (c) INRAE - 2022-2023
#

$DEBUG = isset($_GET['debug']) ? $_GET['debug'] : 0;

if (!isset($SERVER_URL)) $SERVER_URL=$dataverse_urls[0];
if (!isset($API_TOKEN))  $API_TOKEN='';
if (!isset($PARENT))     $PARENT='';

$dataset = '';
if (isset($_GET['dataset'])) $dataset = $_GET['dataset'];

if( isset($_POST['btnpush']) ) {
	$API_TOKEN=$_POST['token'];
	$SERVER_URL=$_POST['server'];
	$PARENT=$_POST['parent'];
	$jsontype = intval($_POST['jsontype']);
	if ( $jsontype==1 && strlen(trim($_POST['dataset'])) )
		$dataset = trim($_POST['dataset']);
}

$EXTERN = isset($_GET['extern']) ? 1 : $EXTERN;

$options_server='';
foreach ($dataverse_urls as $url) {
	$selected=''; if ($url===$SERVER_URL) $selected='selected';
	$options_server .= '<option value="'.$url.'"'.$selected.'>'.$url.'</option>';
}


$array_dataset=array();
$options = array('projection'=>[]);
$options['projection']['title']=true;
$options['projection'][$maggot_fulltitle]=true;
$options['projection']['chemin']=true;

// Rechercher tous les jeux de données répondant aux critères saisis
$obj = dbQueryToArray(array(), $options);
$ret = count($obj);

$options_dataset='<option value="">--- Select a dataset ---</option>';
if ($ret>0) {
	foreach($obj as $arr) {
		$value = (array)$arr;
		if (is_accessible ($value)) continue;
		$array_dataset[$value['title']] = $value['chemin'];
		$selected='';
		if (isset($_GET['dataset']) && $_GET['dataset'] === $value['title']) $selected='selected';
		$options_dataset .= '<option value="'.$value['title'].'" '.$selected.'>'.$value[$maggot_fulltitle].'</option>';
	}
}

?>

<!DOCTYPE html>
<html lang="en" >

<head>
	<title>Maggot - Dataverse</title>

	<?= $HEAD ?>
	<link rel="stylesheet" href="<?= $PATH ?>css/ror-widget.css">
	<script  src="<?= $PATH ?>js/typeahead.min.js"></script>
	<script>var DV_API_URL = "<?= $SERVER_URL.$DV_API ?>";</script>
	<script  src="<?= $PATH ?>js/dv-typeahead.js"></script>

	<script type="text/javascript">
	var DEBUG = <?= $DEBUG ?>;
	function selectjson(id) {
		if (id==1) {
		$('.selectjson1').css('display','flex');
		$('.selectjson2').css('display','none');
		$('#jsontype1').addClass('activated');
		$('#jsontype2').removeClass('activated');
		$('input[name="jsontype"]').val('1');
		} else {
		$('.selectjson1').css('display','none');
		$('.selectjson2').css('display','flex');
		$('#jsontype1').removeClass('activated');
		$('#jsontype2').addClass('activated');
		$('input[name="jsontype"]').val('2');
		}
		$('#errbox').css('display','none');
	}
	function check() {
		var form = document.getElementById("formpush");
		var elements = form.querySelectorAll( "input.solo[type=text]" );
		errmsg = "=== WARNING ===\nThe following field(s) must be defined :\n";
		ret=true;
		obj = { token: "API Token", parent: "Dataverse" }
		for( var i = 0; i < elements.length; ++i ) {
			var element = elements[i];
			var name = element.name;
			if (!Object.keys(obj).includes(name)) { continue; }
			var value = element.value;
			if (value.length>0) { continue; }
			errmsg=errmsg+" "+obj[name]+"\n";
			ret=false;
		}
		if (!ret)
			alert(errmsg);
		else
			$('#waiting').css('display','block');
		$('#errbox').css('display','none');
		return ret;
	}
	$(document).on('click', '#server', function() {
		dataverse_server = $('#server').find(":selected").val();
		DV_API_URL = dataverse_server + '<?= $DV_API ?>';
	})
	</script>
</head>

<body style="zoom: <?= $ZOOMWP ?>">

<?= $header_title ?>
<br><br>

<h1>Publish metadata to a Dataverse repository <a href="<?= $maggot_doc_url.'publish/dataverse' ?>" target="_blank"><i class="bi bi-question-octagon-fill" style="color:grey; font-size: 20px"></i></a></h1>

<section>
	<div id="push" style="display: block; max-width:1200px; margin:auto;">
		<br/>
		<form id="formpush" name="formpush" action="?dataset=<?= $dataset ?>" onsubmit="return check()" autocomplete="off" method="post" enctype="multipart/form-data">
		<fieldset class="form-group2 fieldset_section" style="padding: 20px 40px;">
			<legend class="legend_section"><span style="background-color:white">&nbsp;&nbsp;Submission&nbsp;&nbsp;</span></legend>
			<div class="btn-group gr-jsontype" role="group">
				<button type="button" id="jsontype1" class="btn btn-secondary activated" onclick="selectjson(1);">Internal JSON</button>
				<button type="button" id="jsontype2" class="btn btn-secondary" onclick="selectjson(2);">External JSON</button>
			</div><br><br>
			<div class="input-group mb-3 selectjson1" style="display: flex;">
			<div class="dataset"><b>&nbsp;&nbsp;Dataset:&nbsp;&nbsp;&nbsp;</b></div><select name="dataset" id="dataset" style="height: 38px;" class="form-control form-control-sm dataset-select" onchange="$('#errbox').css('display','none');"><?= $options_dataset ?></select>
			</div> 
			<div class="input-group mb-3 selectjson2" style="display: none;">
				<div class="input-group-prepend">
					<span class="input-group-text">Select a JSON file</span>
				</div>
				<div class="custom-file">
					<input type="file" class="custom-file-input" name="jsonfile" id="jsonfile" onclick="$('#errbox').css('display','none');">
					<label id="label" class="custom-file-label" for="jsonfile"></label>
				</div>
			</div>
			<div style="padding-bottom: 10px;"><b>Dataverse Server URL</b>:&nbsp;<select name="server" id="server" class="form-control form-control-sm"><?= $options_server ?></select></div>
			<div style="padding-bottom: 10px;"><b>API-Token</b>:&nbsp;<input class="form-control form-control-sm solo" type="text" name="token" id="token" value="<?= $API_TOKEN ?>" placeholder="Copy-paste here the API Token from your corresponding Dataverse account" /></div>
			<div style="padding-bottom: 10px;"><b>Dataverse</b>:&nbsp;<input class="form-control form-control-sm solo typeahead" type="text" name="parent" id="parent" value="<?= $PARENT ?>" placeholder="Entrer the first 3 letters of targeted dataverse name"/></div>
			<input type="submit" class="btn btn-primary" style="width: 100px;" id="btnpush" name="btnpush" value="Publish"/>
			<input type="hidden" name="jsontype" id="jsontype" value="1">
			<br>
		</fieldset>
		</form>
	</div>
	<div id="waiting" style="display: none; max-width:1000px; margin:auto;">
		<br><br><img src="<? echo $PATH ?>img/throbber.gif" width="100px">
	</div>
	<div id="result" style="display: block">
		<br>
		<div id='link'></div>
		<div style="padding-bottom: 10px;"><div id="errbox" name="errbox" class="msgbox errbox">
			<div style="margin-left: 2rem; margin-top: 1rem; margin-bottom: 0rem;">
				<span id="errboxmsg">Error !!!</span></p>
			</div>
		</div></div>
		<div style="padding-bottom: 10px;"><div id="errresp" name="errresp" class="msgbox errbox">
			<div style="margin-left: 2rem; margin-top: 1rem; margin-bottom: 0rem;">
				<span id="errrespmsg">Error !!!</span></p>
			</div>
		</div></div>
		<div style="padding-bottom: 10px;"><div id="successresp" name="successresp" class="msgbox successbox">
			<div style="margin-left: 2rem; margin-top: 1rem; margin-bottom: 0rem;">
				<span id="successmsg">Success !!!</span></p>
			</div>
		</div></div>
		<div style="padding-bottom: 10px;"><div id="filemsgbox" name="filemsgbox" class="msgbox filemsgbox">
			<div style="margin-left: 2rem; margin-top: 1rem; margin-bottom: 0rem;">
				<span id="filemsg"></span></p>
			</div>
		</div></div>
	</div>
</section>
<br>
<script>
	$(window).on('load', function() {
		if (<?= $EXTERN ?>) {
			$('.gr-jsontype').css('display','none');
			selectjson(2);
		}
	})
	dvTypeahead();
</script>

<?php

do
{
	// Convert a Maggot JSON to a Dataverse JSON
	if ( ! isset($_POST['btnpush']) ) break;

	$jsontype = intval($_POST['jsontype']);
	if ( $jsontype==1 && ! strlen(trim($_POST['dataset'])) ) {
		$msg='Error: You must specify a dataset !!!';
		echo "<script>$('#errboxmsg').html('".$msg."'); $('#errbox').css('display','block');</script>";
		break;
	}

	if ( $jsontype==2 && ( ! isset($_FILES["jsonfile"]) || $_FILES["jsonfile"]['size']==0 ) ) {
		$msg='Error: You must specify a Maggot JSON file !!!';
		echo "<script>$('#errboxmsg').html('".$msg."'); $('#errbox').css('display','block');</script>";
		break;
	}

	$input_url = 'tmp/'.genKEY().'.json';
	$json_url = 'tmp/'.genKEY().'.json';

	if ($jsontype==1 && strlen(trim($_POST['dataset']))) {
		$path = dirname(str_replace('/pgd_data', '/mnt', $array_dataset[ $_POST['dataset'] ]));
		$name = basename(str_replace('/pgd_data', '/mnt', $array_dataset[ $_POST['dataset'] ]));
		if (is_dir($path) && is_readable($path)) {
			$json = file_get_contents($path.'/'.$name);
			file_put_contents($input_url, $json);
		}
	}

	if ($jsontype==2 && isset($_FILES["jsonfile"])) {
		//if there was an error uploading the file
		if ($_FILES["jsonfile"]["error"] > 0) {
			echo "Return Code: " . $_FILES["jsonfile"]["error"] . "<br />";
			break;
		}

		// the 'jsonfile' file has been submitted
		$name = $_FILES['jsonfile']['name'];
		$ext = strtolower(end(explode('.', $_FILES['jsonfile']['name'])));
		$type = $_FILES['jsonfile']['type'];
		$tmpName = $_FILES['jsonfile']['tmp_name'];
		echo "<script>$('#label').html('".$name."');</script>"; // document.getElementById('label').innerHTML='$name';
	
		$json = file_get_contents($tmpName);
		file_put_contents($input_url, $json);
	}
	
	$metadata = json_decode($json, true);
	if ( gettype($metadata)!== 'array' || count($metadata)==0 ) {
		$msg='Error: '.$name.' is not a valid json file !!!';
		echo "<script>$('#errboxmsg').html('".$msg."'); $('#errbox').css('display','block');</script>";
		break;
	}
//echo "<br><pre>"; print_r ($metadata); echo "</pre><br>";
//echo "<br><pre>"; print_r (array_keys($metadata)); echo "</pre><br>"; 

	// Read Dataverse definition file
	if (! is_file($dataverse_conf)) {
		$msg='Error: '.$dataverse_conf.' does not exist !!!';
		echo "<script>$('#errboxmsg').html('".$msg."'); $('#errbox').css('display','block');</script>";
		break;
	}
	
	$def = tsvToArray($dataverse_conf, true);
	if (! count($def)) {
		$msg='Error: '.$dataverse_conf.' is empty !!!';
		echo "<script>$('#errboxmsg').html('".$msg."'); $('#errbox').css('display','block');</script>";
		break;
	}
	
	// Read Mapping definition file
	if (! is_file($mapping_conf)) {
		$msg='Error: '.$mapping_conf.' does not exist !!!';
		echo "<script>$('#errboxmsg').html('".$msg."'); $('#errbox').css('display','block');</script>";
		break;
	}
	
	$map = tsvToArray($mapping_conf, true);
	if (! count($map)) {
		$msg='Error: '.$mapping_conf.' is empty !!!';
		echo "<script>$('#errboxmsg').html('".$msg."'); $('#errbox').css('display','block');</script>";
		break;
	}

	// check if all "cv" are defined in the mapping
	$errmsg ='';
	$map_names = [];
	foreach( $map as $arr ) array_push($map_names, $arr[0]);
	foreach( $def as $arr1 ) {
		if (!preg_match('/cv:/', $arr1[3])) continue;
		$cvname = explode(':',$arr1[3])[1];
		if (!in_array($cvname, $map_names))
			$errmsg .= 'ERROR: "'.$cvname.'" does not defined in the mapping file<br>';
	}
	if (strlen($errmsg)) {
		echo "<script>$('#errboxmsg').html('".$errmsg."'); $('#errbox').css('display','block');</script>";
		break;
	}

	// Convert JSON to Dataverse
	$outjson= json2dataverse($metadata, $def, $map);
	if (strlen($outjson)==0) {
		$msg='Error: Either no conversion has been done or the conversion has failed !!!';
		echo "<script>$('#errboxmsg').html('".$msg."'); $('#errbox').css('display','block');</script>";
		break;
	}

	// Display links to input/output JSON files and both Dataverse & Mapping definition files
	$ds=$metadata['title'];
	file_put_contents($json_url, $outjson);
	
	// Display links of both Dataverse & Mapping definition files
	$output1 = 'tmp/'.genKEY().'.html';
	$output2 = 'tmp/'.genKEY().'.html';
	file_put_contents($output1, tsv2html($PATH.$dataverse_conf));
	file_put_contents($output2, tsv2html($PATH.$PATH.$mapping_conf));
	
	$arr_links = array();
	$arr_links[0]=array('Input file',$PATH.$input_url,$name);
	$arr_links[1]=array('Dataverse definition file',$output1,$dataverse_conf);
	$arr_links[2]=array('Mapping definition file',$output2,$mapping_conf);
	$arr_links[3]=array('Output file',$PATH.$json_url,'dataverse_'.$ds.'.json');

	// Test if a dataset is already present in the data repository
	$persistentid=''; $state='';
	$SEARCH_OPTIONS  ='q=Maggot&show_facets=true&fq=otherIdAgency:Maggot&fq=otherIdValue:'.otherID($ds);
	$SEARCH_OPTIONS .='&sort=name&order=desc&type=dataset';
	$output_arr = get_api_dataverse('search?'.$SEARCH_OPTIONS);
	if (gettype($output_arr) !== 'array' || ! array_key_exists('data', $output_arr)) {
		$msg = 'ERROR: The data repository '.$SERVER_URL.' seemns unreachable or unavailable !!!';
		echo "<script>$('#errboxmsg').html('".$msg."'); $('#errbox').css('display','block');</script>";
		break;
	} else if ($output_arr['data']['total_count']) {
		$arr=$output_arr['data']['items'][0];
		$persistentid=$arr['global_id'];
		$state = $arr['versionState'];
		$infos = 'Version state: '.$state;
		if ($state==='RELEASED') $infos .= ' '.$arr['majorVersion'].'.'.$arr['minorVersion'];
		$infos .= ', create at '.$arr['createdAt'].', '.$persistentid.', dataverse: '.$arr['identifier_of_dataverse'];
		$msg='WARNING: The dataset '.$ds.' already exists in the data repository '.$SERVER_URL.'.';
		$msg.='<br>&nbsp;&nbsp;&nbsp;'.$infos; 
		//echo "<script>$('#errboxmsg').html('".$msg."'); $('#errbox').css('display','block');</script>";
	}

	// Create / Update dataset to the data repository
	if (strlen($persistentid)) {
		// Update
		try { 
			$new_ds = json_decode($outjson,true)['datasetVersion']; 
			unset($new_ds['license']);
			unset($new_ds['termsOfUse']);
			$new_jsonds = json_encode($new_ds);
			$json_url = 'tmp/'.genKEY().'.json';
			file_put_contents($json_url, $new_jsonds);
			$request="datasets/:persistentId/versions/:draft?persistentId=".$persistentid;
			$output_arr = send_api_dataverse($request,$json_url, $mode='PUT');
		} catch(Exception $e) { $output_arr=[]; }
	} else {
		// Create
		$output_arr = send_api_dataverse("dataverses/$PARENT/datasets",$json_url, $mode='POST');
	}
//echo "<br><pre>"; print_r ($output_arr); echo "</pre><br>"; 

	if (empty($output_arr)) {
		$htmloutput = "Error: There is a problem with the JSON file";
		echo "<script>$('#errresp').css('display','block'); $('#errrespmsg').html('".$htmloutput."');</script>";
		break;
	}

	if (!isset($output_arr['status'])) {
		if (strlen($persistentid)) {
			$htmloutput="ERROR: an error occurs when trying to update the dataset with the persistentid=".$persistentid;
		} else {
			$htmloutput="ERROR: an error occurs when trying to create the dataset ";
		}
		$htmloutput .= "<br>Either there is a problem with the JSON file, or try again later";
		echo "<script>$('#errresp').css('display','block'); $('#errrespmsg').html('".$htmloutput."');</script>";
		break;
	}

	$status = $output_arr['status'];
	if ($status==='ERROR') {
		$htmloutput="ERROR: ".addslashes($output_arr['message']);
		echo "<script>$('#errresp').css('display','block'); $('#errrespmsg').html('".$htmloutput."');</script>";
		break;
	}

	echo "<script>$('#push').css('display','none');</script>";
	echo "<script>$('#link').html('".box_links($arr_links)."');</script>";

	if ($status==='OK') {
		$array_files =array();
		if (isset($output_arr['data']['datasetPersistentId'])) {
			$persistentId=$output_arr['data']['datasetPersistentId'];
			$datasetID = $output_arr['data']['datasetId'];
			$op='updated';
			if (isset($output_arr['data']['files']))
				foreach( $output_arr['data']['files'] as $arr)
					$array_files[ $arr['dataFile']['filename'] ] = $arr['dataFile']['id'];
		} else if (isset($output_arr['data']['persistentId'])) {
			$persistentId=$output_arr['data']['persistentId']; 
			$datasetID = $output_arr['data']['id'];
			$op='created';
		}
	
		$htmlsuccess="SUCCESS: The dataset ".$ds." has been ".$op." with success with the persistent ID: ".$persistentId; // ' (id = '.$datasetID.')';
		echo "<script>$('#successmsg').html('".$htmlsuccess."'); $('#successresp').css('display','block');</script>";
	
		// Sending file if internal JSON
		if ($jsontype==1) {
			$msg = send_resources_files($datasetID, $metadata['resources'], $path, $array_files);
			if (strlen($msg)) {
				echo "<script>$('#filemsg').html('".$msg."'); $('#filemsgbox').css('display','block');</script>";
			}
		}
	
		// Create a private URL if not already exists
		$output_arr = privateURL_api_dataverse($datasetID,1);
		$status = $output_arr['status'];
		if ($status==='ERROR') {
			$output_arr = privateURL_api_dataverse($datasetID,0);
			$status = $output_arr['status'];
			if ($status==='ERROR') {
				$htmloutput="ERROR: ".addslashes($output_arr['message']);
				echo "<script>$('#errresp').css('display','block'); $('#errrespmsg').html('".$htmloutput."');</script>";
				break;
			}
		}
		if ($status==='OK') {
			$link = $output_arr['data']['link'];
			$astyle = 'color: #2b5165; text-decoration: none;';
			$htmlsuccess .= "<br>The corresponding private URL is <a style=\"".$astyle."\" href=\"".$link."\" target=\"_blank\">".$link."</a>";
			echo "<script>$('#successmsg').html('".$htmlsuccess."'); $('#successresp').css('display','block');</script>";
		}
	}

} while(0);

?>

<script>
	document.querySelector('.custom-file-input').addEventListener('change',function(e){
		var fileName = document.getElementById("jsonfile").files[0].name;
		var nextSibling = e.target.nextElementSibling
		nextSibling.innerText = fileName
	});
	$('#waiting').css('display','none');
</script>

<br>
