<!DOCTYPE html>
<html lang="en" >

<head>
	<title>Maggot - Config</title>
	<?php echo $HEAD ?>
</head>

<body style="zoom: <? echo $ZOOMWP ?>">

<?php echo $header_config ?>
<br><br>

<h1>View all current definition files <a href="<? echo $maggot_doc_url.'definitions/' ?>" target="doc"><i class="bi bi-question-octagon-fill" style="color:grey; font-size: 20px"></i></a></h1>
<br>

<section>
	<form id="formconvert" name="formconvert" action="#" autocomplete="off" method="post" enctype="multipart/form-data">
		<br>
		<div id='link' class="mb-3"></div>
		<br>
		<div id="dwnbtn"></div><br>
		<div style="padding-bottom: 10px;"><div id="errbox" name="errbox" class="msgbox errbox">
			 <div style="margin-left: 2rem; margin-top: 1rem; margin-bottom: 0rem;">
				  <span id="errboxmsg">Error !!!</span></p>
			 </div>
		</div></div>
	</form>
</section>
<br>

<?php

do
{
	$json_url = 'conf/config_terms.json';
	$terms_url = 'conf/config_terms.txt';
	$doc_url = 'conf/config_doc.txt';
	$schema_url = 'conf/maggot-schema.json';

	$msg = '';

	$arr_links = array();
	$nb_links = 0;

	// Read Terminology definition file
	if (! is_file($json_url)) {
		$msg='Error: '.$json_url.' does not exist !!!';
		echo "<script>$('#errboxmsg').html('".$msg."'); $('#errbox').css('display','block');</script>";
		break;
	} else {
		// Display link to the Terminology definition file
		$output_txt = 'tmp/'.genKEY().'.txt';
		$output1 = 'tmp/'.genKEY().'.html';
		file_put_contents($output_txt, json2tsv());
		file_put_contents($output1, tsv2html($output_txt));
		$arr_links[$nb_links]=array('Terminology definition file',$PATH.$output1,$terms_url,$PATH.$output_txt,"Terminology", $maggot_doc_url.'definitions/terminology/');
		$nb_links++;
		if ($DEV) {
			$output1b = 'tmp/'.genKEY().'.json';
			file_put_contents($output1b, tsv2schema($output_txt, $url_schema));
			$arr_links[$nb_links]=array('JSON Schema file',$PATH.$output1b,$schema_url,$PATH.$output1b,"JSON Schema", $maggot_doc_url.'definitions/terminology/');
			$nb_links++;
		}
	}

	// Read Documentation definition file
	if (! is_file($doc_url)) {
		$msg .= 'Warning: '.$doc_url.' does not exist !!!<br>';
	} else {
		// Display link to the Dataverse definition files
		$output2 = 'tmp/'.genKEY().'.html';
		file_put_contents($output2, tsv2html($doc_url));
		$arr_links[$nb_links]=array('Documentation definition file',$PATH.$output2,$doc_url,$PATH.$doc_url, "Documentation", $maggot_doc_url.'definitions/terminology/');
		$nb_links++;
	}

	// Read Mapping definition file
	if (! is_file($mapping_conf)) {
		$msg .= 'Warning: '.$mapping_conf.' does not exist !!!<br>';
	} else {
		// Display link to the Mapping definition files
		$output4 = 'tmp/'.genKEY().'.html';
		file_put_contents($output4, tsv2html($mapping_conf));
		$arr_links[$nb_links]=array('Mapping definition file',$PATH.$output4,$mapping_conf,$PATH.$mapping_conf, "Mapping", $maggot_doc_url.'definitions/mapping/');
		$nb_links++;
	}

	// Read Dataverse definition file
	if($export_dataverse)
		if (! is_file($dataverse_conf)) {
			$msg .= 'Warning: '.$dataverse_conf.' does not exist !!!<br>';
		} else {
			// Display link to the Dataverse definition files
			$output3 = 'tmp/'.genKEY().'.html';
			file_put_contents($output3, tsv2html($dataverse_conf));
			$arr_links[$nb_links]=array('Dataverse definition file',$PATH.$output3,$dataverse_conf,$PATH.$dataverse_conf,"Dataverse", $maggot_doc_url.'definitions/dataverse/');
			$nb_links++;
		}

	// Read Zenodo definition file
	if($export_zenodo)
		if (! is_file($zenodo_conf)) {
			$msg .= 'Warning: '.$zenodo_conf.' does not exist !!!<br>';
		} else {
			// Display link to the Mapping definition files
			$output6 = 'tmp/'.genKEY().'.html';
			file_put_contents($output6, tsv2html($zenodo_conf));
			$arr_links[$nb_links]=array('Zenodo definition file',$PATH.$output6,$zenodo_conf,$PATH.$zenodo_conf, "Zenodo", $maggot_doc_url.'definitions/zenodo/');
			$nb_links++;
		}

	// Read Schema definition file
	if($export_jsonld)
		if (! is_file($jsonld_conf)) {
			$msg .= 'Warning: '.$jsonld_conf.' does not exist !!!<br>';
		} else {
			// Display link to the Mapping definition files
			$output5 = 'tmp/'.genKEY().'.html';
			file_put_contents($output5, tsv2html($jsonld_conf));
			$arr_links[$nb_links]=array('JSON-LD definition file',$PATH.$output5,$jsonld_conf,$PATH.$jsonld_conf, "JSON-LD", $maggot_doc_url.'definitions/json-ld/');
			$nb_links++;
		}

	// OAI-PMH definition file
	if($export_oai)
		if (! is_file($oaipmh_conf)) {
			$msg .= 'Warning: '.$oaipmh_conf.' does not exist !!!<br>';
		} else {
			// Display link to the Mapping definition files
			$output7 = 'tmp/'.genKEY().'.html';
			file_put_contents($output7, tsv2html($oaipmh_conf));
			$arr_links[$nb_links]=array('OAI-PMH definition file',$PATH.$output7,$oaipmh_conf,$PATH.$oaipmh_conf, "OAI-PMH", $maggot_doc_url.'definitions/oai-pmh/');
			$nb_links++;
		}

	echo "<script>$('#link').html('".box_links($arr_links,1,1)."');</script>";
	if (strlen($msg)>0)
		echo "<script>$('#errboxmsg').html('".$msg."'); $('#errbox').css('display','block');</script>";

} while(0);

?>

<br>

<script>
	$("[name='btn-conv2']").removeClass('btn-primary');  $("[name='btn-conv2']").addClass('menu-select');
</script>
