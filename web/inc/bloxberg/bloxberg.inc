<?php
#
# Bloxberg Blockchain implementation 
#
# Auteur(s): D.JACOB 
# Copyright: (c) INRAE - 2022-2023
#

if (!isset($BC_SERVER_URL)) $BC_SERVER_URL=$bloxberg_urls[0];
if (!isset($BC_API_TOKEN))  $BC_API_TOKEN=''; //genKEY();
if (!isset($PUBLICKEY))  $PUBLICKEY=$BB_PUBLICKEY;

$dataset = '';
if (isset($_GET['dataset'])) $dataset = $_GET['dataset'];

if( isset($_POST['btnpush']) ) {
	$BC_API_TOKEN=$_POST['token'];
	$BC_SERVER_URL=$_POST['server'];
	$PUBLICKEY=$_POST['publickey'];
	$jsontype = intval($_POST['jsontype']);
	if ( $jsontype==1 && strlen(trim($_POST['dataset'])) )
		$dataset = trim($_POST['dataset']);
}

$EXTERN = isset($_GET['extern']) ? 1 : $EXTERN;

$options_server='';
foreach ($bloxberg_urls as $url) {
	$selected=''; if ($url===$BC_SERVER_URL) $selected='selected';
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
		if ($dataset === $value['title']) $selected='selected';
		$options_dataset .= '<option value="'.$value['title'].'" '.$selected.'>'.$value[$maggot_fulltitle].'</option>';
	}
}

?>

<!DOCTYPE html>
<html lang="en" >

<head>
	<title>Maggot - bloxberg blockchain</title>

	<?php echo $HEAD ?>

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
		obj = { token: "API bloxberg", publickey: "Wallet bloxberg" }
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
	</script>
</head>

<body style="zoom: <? echo $ZOOMWP ?>">

<?php echo $header_title ?>
<br><br>

<h1>EXPERIMENTAL - bloxberg Blockchain <a href="<? echo $maggot_doc_url.'bloxberg' ?>" target="_blank"><i class="bi bi-question-octagon-fill" style="color:grey; font-size: 20px"></i></a></h1>
<br>

<section>
	<div id="push" style="display: block; max-width:1000px; margin:auto;">
		<br/>
		<form id="formpush" name="formpush" action="?dataset=<? echo $dataset ?>" onsubmit="return check()" autocomplete="off" method="post" enctype="multipart/form-data">
			<h2>Certification of metadata file on the bloxberg blockchain</h2>
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
			<div style="padding-bottom: 10px;"><b>bloxberg Server URL</b>:&nbsp;<select name="server" id="server" class="form-control form-control-sm"><? echo $options_server ?></select></div>
			<div style="padding-bottom: 10px;"><b>Wallet bloxberg</b>:&nbsp;<input class="form-control form-control-sm solo" type="text" name="publickey" id="publickey" value="<? echo $PUBLICKEY ?>" /></div>
			<div style="padding-bottom: 10px;"><b>API bloxberg</b>:&nbsp;<input class="form-control form-control-sm solo" type="text" name="token" id="token" value="<? echo $BC_API_TOKEN ?>" /></div>
			<input type="submit" class="btn btn-primary" style="width: 100px;" id="btnpush" name="btnpush" value="Publish"/>
			<input type="hidden" name="jsontype" id="jsontype" value="1">
			<br>
		</form>
	</div>
	<div id="waiting" style="display: none; max-width:1000px; margin:auto;">
		<br><br><img src="<? echo $PATH ?>img/throbber.gif" width="100px">
	</div>
	<div id="result" style="display: block">
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
</script>

<?php

do
{
	// Push a Maggot JSON to the bloxberg blockchain
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
	$BBjson_url = 'tmp/'.genKEY().'.json';
	$Upload_url = 'tmp/'.genKEY().'.json';
	$RESPjson_url = 'tmp/'.genKEY().'.json';
	$ZIP_url = 'tmp/'.genKEY().'.zip';

	if ($jsontype==1 && strlen(trim($_POST['dataset']))) {
		$path = dirname(str_replace('/pgd_data', '/mnt', $array_dataset[ $_POST['dataset'] ]));
		$name = basename(str_replace('/pgd_data', '/mnt', $array_dataset[ $_POST['dataset'] ]));
		if (!file_exists($path.'/'.$auth_bloxberg_file)) {
			$msg='Error: Certification of the metadata in the bloxberg blockchain is not allowed for this dataset !!!';
			echo "<script>$('#errboxmsg').html('".$msg."'); $('#errbox').css('display','block');</script>";
			break;
		}
		if (is_dir($path) && is_readable($path)) {
			$meta_json = file_get_contents($path.'/'.$name);
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
	
		$meta_json = file_get_contents($tmpName);
	}

	$metadata = json_decode($meta_json, true);
	if ( gettype($metadata)!== 'array' || count($metadata)==0 ) {
		$msg='Error: '.$name.' is not a valid json file !!!';
		echo "<script>$('#errboxmsg').html('".$msg."'); $('#errbox').css('display','block');</script>";
		break;
	}
	file_put_contents($input_url, $meta_json);

//echo "<br><pre>"; print_r ($metadata); echo "</pre><br>";
//echo "<br><pre>"; print_r (array_keys($metadata)); echo "</pre><br>"; 


	// Create the bloxberg JSON file
	$BBjson= json2bloxberg($metadata);
	if (strlen($BBjson)==0) {
		$msg='Error: Creation of the bloxberg JSON file failed !!!';
		echo "<script>$('#errboxmsg').html('".$msg."'); $('#errbox').css('display','block');</script>";
		break;
	}
	file_put_contents($BBjson_url, $BBjson);

	# bloxberg API calling  to certify the metadata file on the bloxberg blockchain.
	$upload_js = get_json_request($BBjson,$meta_json);
	$response_js = certify_api_bloxberg($upload_js);
	file_put_contents($Upload_url, $upload_js);
	file_put_contents($RESPjson_url, $response_js);

//$test_url = 'tmp/bloxberg_json_response.json';
//if (is_readable($test_url)) $response_js = file_get_contents($test_url);
//file_put_contents($RESPjson_url, $response_js);

	// Display links to input/output JSON files
	$ds=$metadata['title'];
	$path='tmp/';
	$arr_links = array();
	$arr_links[0]=array('Metadata file',$PATH.$input_url,$name,$PATH.$input_url,"Metadata");
	$arr_links[1]=array('bloxberg metadata',$PATH.$BBjson_url,'bloxberg_metadata_'.$ds.'.json',$PATH.$BBjson_url,"Metadata");
	$arr_links[2]=array('bloxberg query',$PATH.$Upload_url,'bloxberg_query_'.$ds.'.json',$PATH.$Upload_url,"Query");
	$arr_links[3]=array('bloxberg response',$PATH.$RESPjson_url,'bloxberg_response_'.$ds.'.json',$PATH.$RESPjson_url,"Response");
	$flglink=0;
	echo "<script>$('#push').css('display','none');</script>";
	echo "<script>$('#link').html('".box_links($arr_links, $flglink)."');</script>";

	$output_arr = json_decode($response_js, true);
//if (! is_readable($test_url)) {
	if (is_array($output_arr) && array_key_exists('detail', $output_arr) && strlen($output_arr['detail'])>0) {
		$msg='Error: Fail to certify the metadata file on the bloxberg blockchain!!!';
		$msg.= "<br><pre>".$output_arr['detail']."</pre>";
		echo "<script>$('#errboxmsg').html('".$msg."'); $('#errbox').css('display','block');</script>";
		break;
	}

	# CURL request to the bloxberg API (/generatePDF)
	$output_js = generatePDF_api_bloxberg($response_js, $ZIP_url);
	if (file_exists($ZIP_url)) {
		$arr_links[4]=array('bloxberg certificate',$PATH.$ZIP_url,'bloxberg_certificate_'.$ds.'.zip',$PATH.$ZIP_url,"Certificate");
		$htmlsuccess = "Success : the metadata file has been certified on the bloxberg blockchain";
		$htmlsuccess.= "<br>Download the ZIP which contains the corresponding certificate as a PDF file";
		echo "<script>$('#successmsg').html('".$htmlsuccess."'); $('#successresp').css('display','block');</script>";
		$flglink=1;
	} else {
		$error_url = 'tmp/'.genKEY().'.json';
		file_put_contents($error_url, $output_js);
		$arr_links[4]=array('bloxberg error',$PATH.$error_url,'bloxberg_error_'.$ds.'.json',$PATH.$error_url,"Error");
		$msg='Error: Fail to generate the PDF of the bloxberg certificate !!!';
		echo "<script>$('#errboxmsg').html('".$msg."'); $('#errbox').css('display','block');</script>";
		$flglink=0;
	}

	echo "<script>$('#push').css('display','none');</script>";
	echo "<script>$('#link').html('".box_links($arr_links, $flglink)."');</script>";

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
