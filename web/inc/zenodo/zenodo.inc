<?php
#
# Zenodo implementation 
# Preliminary work (sept 2023)
#
# Auteur(s): D.JACOB 
# Copyright: (c) INRAE - 2022-2023
#

if (!isset($ZENODO_SERVER_URL))  $ZENODO_SERVER_URL=$zenodo_urls[0];
if (!isset($ZENODO_API_TOKEN))   $ZENODO_API_TOKEN='';
if (!isset($COMMUNITIES))        $COMMUNITIES='zenodo';

$dataset = '';
if (isset($_GET['dataset'])) $dataset = $_GET['dataset'];

if( isset($_POST['btnpush']) ) {
	$ZENODO_API_TOKEN=$_POST['token'];
	$ZENODO_SERVER_URL=$_POST['server'];
	$COMMUNITIES=$_POST['parent'];
	$jsontype = intval($_POST['jsontype']);
	if ( $jsontype==1 && strlen(trim($_POST['dataset'])) )
		$dataset = trim($_POST['dataset']);
}

$EXTERN = isset($_GET['extern']) ? 1 : $EXTERN;

$options_server='';
foreach ($zenodo_urls as $url) {
	$selected=''; if ($url===$ZENODO_SERVER_URL) $selected='selected';
	$options_server .= '<option value="'.$url.'"'.$selected.'>'.$url.'</option>';
}


$array_dataset=array();
$options = array('projection'=>[]);
$options['projection']['title']=true;
$options['projection'][$maggot_fulltitle]=true;
$options['projection']['chemin']=true;

// Rechercher tous les jeux de données répondant aux critères saisis
$query = new MongoDB\Driver\Query(array(), $options);
$obj = $client->executeQuery('pgd-db.metadata', $query)->toArray();
$ret = count($obj);

$options_dataset='<option value="">---</option>';
if ($ret>0) {
	foreach($obj as $arr) {
		$value = (array)$arr;
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
	<title>Maggot - Zenodo</title>

	<?php echo $HEAD ?>
	<link rel="stylesheet" href="<?php echo $PATH ?>css/ror-widget.css">
	<script  src="<?php echo $PATH ?>js/typeahead.min.js"></script>
	<script>var ZC_API_URL = "<?php echo $ZENODO_SERVER_URL.$ZC_API ?>";</script>
	<script  src="<?php echo $PATH ?>js/zc-typeahead.js"></script>

	<script type="text/javascript">
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
		obj = { token: "API Token" }
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
		zenodo_server = $('#server').find(":selected").val();
		ZC_API_URL = zenodo_server + '<?php echo $ZC_API ?>';
	})
	</script>
</head>

<body style="zoom: <? echo $ZOOMWP ?>">

<?php echo $header_title ?>
<br><br>

<h1>Publishing Datasets <a href="<? echo $maggot_doc_url.'publish/zenodo' ?>" target="_blank"><i class="bi bi-question-octagon-fill" style="color:grey; font-size: 20px"></i></a></h1>
<br>

<section>
	<div id="push" style="display: block; max-width:1000px; margin:auto;">
		<br/>
		<form id="formpush" name="formpush" action="?dataset=<? echo $dataset ?>" onsubmit="return check()" autocomplete="off" method="post" enctype="multipart/form-data">
			<h2>Publish metadata to a Zenodo repository</h2>
			<div class="btn-group gr-jsontype" role="group">
				<button type="button" id="jsontype1" class="btn btn-secondary activated" onclick="selectjson(1);">Internal JSON</button>
				<button type="button" id="jsontype2" class="btn btn-secondary" onclick="selectjson(2);">External JSON</button>
			</div><br><br>
			<div class="input-group mb-3 selectjson1" style="display: flex;">
			<div class="dataset"><b>&nbsp;&nbsp;Dataset:&nbsp;&nbsp;&nbsp;</b></div><select name="dataset" id="dataset" style="height: 38px;" class="form-control form-control-sm dataset-select" onchange="$('#errbox').css('display','none');"><? echo $options_dataset ?></select>
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
			<div style="padding-bottom: 10px;"><b>Zenodo Server URL</b>:&nbsp;<select name="server" id="server" class="form-control form-control-sm"><? echo $options_server ?></select></div>
			<div style="padding-bottom: 10px;"><b>API-Token</b>:&nbsp;<input class="form-control form-control-sm solo" type="text" name="token" id="token" value="<? echo $ZENODO_API_TOKEN ?>" /></div>
			<div style="padding-bottom: 10px;"><b>Communities</b>:&nbsp;<input class="form-control form-control-sm solo typeahead" type="text" name="parent" id="parent" value="<? echo $COMMUNITIES ?>" /></div>
			<input type="submit" class="btn btn-primary" style="width: 100px;" id="btnpush" name="btnpush" value="Publish"/>
			<input type="hidden" name="jsontype" id="jsontype" value="1">
			<br>
		</form>
	</div>
	<div id="waiting" style="display: none; max-width:1000px; margin:auto;">
		<br><br><img src="img/throbber.gif" width="100px">
	</div>
	<div id="result" style="display: block; max-width: 1000px; margin: auto;">
		<br>
		<div id='link'></div>
		<br>
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
		if (<? echo $EXTERN ?>) {
			$('.gr-jsontype').css('display','none');
			selectjson(2);
		}
	})
	zcTypeahead();
</script>

<?php

do
{
	// Convert a Maggot JSON to a Zenodo JSON
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

	// Read Zenodo definition file
	if (! is_file($zenodo_conf)) {
		$msg='Error: '.$zenodo_conf.' does not exist !!!';
		echo "<script>$('#errboxmsg').html('".$msg."'); $('#errbox').css('display','block');</script>";
		break;
	}

	$def = tsvToArray($zenodo_conf, false);
	if (! count($def)) {
		$msg='Error: '.$zenodo_conf.' is empty !!!';
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

//echo "<br><pre>"; print_r ($def); echo "</pre><br>";
//echo "<br><pre>"; print_r (array_keys($metadata)); echo "</pre><br>"; 

	// Convert JSON to Dataverse
	$outjson = json2zenodo($metadata, $def, $map, $COMMUNITIES);
	if (strlen($outjson)==0) {
		$msg='Error: Either no conversion has been done or the conversion has failed !!!';
		echo "<script>$('#errboxmsg').html('".$msg."'); $('#errbox').css('display','block');</script>";
		break;
	}
	
	// Display links to input/output JSON files and both Dataverse & Mapping definition files
	file_put_contents($json_url, $outjson);
	
	// Display links of both Dataverse & Mapping definition files
	$output1 = 'tmp/'.genKEY().'.html';
	$output2 = 'tmp/'.genKEY().'.html';
	file_put_contents($output1, tsv2html($PATH.$zenodo_conf));
	//file_put_contents($output2, tsv2html($PATH.$PATH.$mapping_conf));

	$ds=$metadata['title'];
	$arr_links = array();
	$arr_links[0]=array('Zendo definition file',$output1,$zenodo_conf, $PATH.$zenodo_conf, "Zenodo");
	$arr_links[1]=array('Input file',$PATH.$input_url,$name, $PATH.$input_url, "Input");
	$arr_links[2]=array('Output file',$PATH.$json_url,'zenodo_'.$ds.'.json',$PATH.$json_url, "Output");
	
	echo "<script>$('#push').css('display','none');</script>";
	echo "<script>$('#link').html('".box_links($arr_links,0)."');</script>";

	// Test if a dataset is already present in the data repository
	$ds=$metadata['fulltitle'];
	$output_arr = get_api_zenodo();
	$html_record_id = $record_id = $bucket_link = '';
	if (is_array($output_arr) && count($output_arr)>0) {
		for($k=0; $k<count($output_arr); $k++) {
			$arr=$output_arr[$k];
			//if (strlen($arr['doi'])==0 && $arr['submitted']==0 && $arr['title']==$ds) {
			if ($arr['title']==$ds) {
				$record_id = $arr['record_id'];
				$html_record_id = $arr['links']['html'];
				$state = $arr['state'];
				break;
			}
		}
	}

	// Create
	if (strlen($record_id)==0) {
		$output_arr = send_api_zenodo($outjson, $mode='POST');
	} else {
		$astyle = 'color: #2b5165; text-decoration: none;';
		$htmlsuccess="WARNING: The dataset ".$ds." has been already created with the record ID: ".$record_id;
		$htmlsuccess .= "<br>The corresponding URL is <a style=\"".$astyle."\" href=\"".$html_record_id."\" target=\"_blank\">".$html_record_id."</a>";
		if ($state == 'done')
			$htmlsuccess .= "<br>WARNING: This dataset has already been published, so you cannot modify by this way";
		echo "<script>$('#successmsg').html('".$htmlsuccess."'); $('#successresp').css('display','block');</script>";
		if ($state == 'done')
			break;
	}

//echo "<br><pre>"; print_r ($output_arr); echo "</pre><br>";

	// Error
	if (is_array($output_arr) && count($output_arr)>0 && array_key_exists('status',$output_arr)) {
		$status=$output_arr['status']; $message = str_replace("'","\\'", $output_arr['message']);
		$htmloutput="Status: $status - $message<br>";
		if (array_key_exists('errors',$output_arr))
			for($k=0; $k<count($output_arr['errors']); $k++) {
				$arr=$output_arr['errors'][$k];
				$field = $arr['field']; $message = str_replace("'","\\'", $arr['message']);
				$htmloutput .= "&nbsp;&nbsp;&nbsp;$k : $field - $message<br>";
			}
		echo "<script>$('#errresp').css('display','block'); $('#errrespmsg').html('".$htmloutput."');</script>";
		break;
	}
	
	// Success
	if (strlen($record_id)==0 && is_array($output_arr)) {
		$link = $output_arr['links']['html'];
		$bucket_link = $output_arr['links']['bucket'];
		$record_id = $output_arr['record_id'];
		$astyle = 'color: #2b5165; text-decoration: none;';
		$htmlsuccess="SUCCESS: The dataset ".$ds." has been created with success with the record ID: ".$record_id;
		$htmlsuccess .= "<br>The corresponding URL is <a style=\"".$astyle."\" href=\"".$link."\" target=\"_blank\">".$link."</a>";
		echo "<script>$('#successmsg').html('".$htmlsuccess."'); $('#successresp').css('display','block');</script>";
	}

	if (!file_exists($path.'/'.$auth_senddata_file)) break;

	// Sending file if internal JSON
	if ($jsontype==1) {
		$files = array();
		foreach( $metadata['resources'] as $arr ) {
			$file = $path.'/'.$arr['location'];
			if (strlen($arr['location'])>0 && ! preg_match('/^https?:/',$arr['location']) && file_exists($file))
				array_push($files, $file);
		}
		if (count($files)>0) {
			$msg = '';
			if (strlen($bucket_link) == 0) {
				$output_arr = get_api_zenodo($record_id);
				$bucket_link = $output_arr['links']['bucket'];
			}
			foreach( $files as $file) {
				$output_arr = send_file_zenodo($bucket_link, $file);
				if (array_key_exists('status',$output_arr))
					$msg .='ERROR '.$arr['location'].': '.addslashes($output_arr['message']).'<br>';
			}
			if (strlen($msg)) {
				echo "<script>$('#filemsg').html('".$msg."'); $('#filemsgbox').css('display','block');</script>";
			}
		}
	}
} while(0);

echo "<script>$('#waiting').css('display','none');</script>";

?>

<br>

<script>
    /* Montrer le nom du fichier selectionner pour chargement : https://www.codeply.com/p/LtpNZllird */
    document.querySelector('.custom-file-input').addEventListener('change',function(e){
      var fileName = document.getElementById("jsonfile").files[0].name;
      var nextSibling = e.target.nextElementSibling
      nextSibling.innerText = fileName
    });
</script>
