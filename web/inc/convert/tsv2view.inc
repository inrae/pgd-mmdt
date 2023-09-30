<!DOCTYPE html>
<html lang="en" >

<head>
	<title>Maggot - Config</title>
	<?php echo $HEAD ?>
</head>

<body style="zoom: <? echo $ZOOMWP ?>">

<?php echo $header_config ?>
<br><br>

<h1>View all current definition files</h1>
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

	$html_dwnbtn = '<font size="+1"><b style="color:#557ca7;">Download : </b></font>&nbsp;&nbsp;';
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
		$arr_links[$nb_links]=array('Terminology definition file',$PATH.$output1,$terms_url);
		$html_dwnbtn .= "<input id=\"dwn1\" type=\"download\" value=\"Terminology\" class=\"btn btn-success btn-xs\" style=\"width: 200px;\" onclick=\"downloadFile(\'".$PATH.$output_txt."\',\'config_terms.txt\');\"/>&nbsp;&nbsp;";
		$nb_links++;
	}

	// Read Documentation definition file
	if (! is_file($doc_url)) {
		$msg .= 'Warning: '.$doc_url.' does not exist !!!<br>';
	} else {
		// Display link to the Dataverse definition files
		$output2 = 'tmp/'.genKEY().'.html';
		file_put_contents($output2, tsv2html($doc_url));
		$arr_links[$nb_links]=array('Documentation definition file',$PATH.$output2,$doc_url);
		$html_dwnbtn .= "<input id=\"dwn1\" type=\"download\" value=\"Documentation\" class=\"btn btn-success btn-xs\" style=\"width: 200px;\" onclick=\"downloadFile(\'".$PATH.$doc_url."\', \'config_doc.txt\');\"/>&nbsp;&nbsp;";
		$nb_links++;
	}

	// Read Mapping definition file
	if (! is_file($mapping_conf)) {
		$msg .= 'Warning: '.$mapping_conf.' does not exist !!!<br>';
	} else {
		// Display link to the Mapping definition files
		$output4 = 'tmp/'.genKEY().'.html';
		file_put_contents($output4, tsv2html($mapping_conf));
		$arr_links[$nb_links]=array('Mapping definition file',$PATH.$output4,$mapping_conf);
		$html_dwnbtn .= "<input id=\"dwn1\" type=\"download\" value=\"Mapping\" class=\"btn btn-success btn-xs\" style=\"width: 200px;\" onclick=\"downloadFile(\'".$PATH.$mapping_conf."\',\'mapping.txt\');\"/>&nbsp;&nbsp;";
		$nb_links++;
	}

	// Read Dataverse definition file
	if (! is_file($dataverse_conf)) {
		$msg .= 'Warning: '.$dataverse_conf.' does not exist !!!<br>';
	} else {
		// Display link to the Dataverse definition files
		$output3 = 'tmp/'.genKEY().'.html';
		file_put_contents($output3, tsv2html($dataverse_conf));
		$arr_links[$nb_links]=array('Dataverse definition file',$PATH.$output3,$dataverse_conf);
		$html_dwnbtn .= "<input id=\"dwn1\" type=\"download\" value=\"Dataverse\" class=\"btn btn-success btn-xs\" style=\"width: 200px;\" onclick=\"downloadFile(\'".$PATH.$dataverse_conf."\',\'dataverse.txt\');\"/>&nbsp;&nbsp;";
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
			$arr_links[$nb_links]=array('Zenodo definition file',$PATH.$output6,$zenodo_conf);
			$nb_links++;
		}

	// Read Schema definition file
	if($export_schema_org)
		if (! is_file($schema_conf)) {
			$msg .= 'Warning: '.$schema_conf.' does not exist !!!<br>';
		} else {
			// Display link to the Mapping definition files
			$output5 = 'tmp/'.genKEY().'.html';
			file_put_contents($output5, tsv2html($schema_conf));
			$arr_links[$nb_links]=array('Schema.org JSON-LD definition file',$PATH.$output5,$schema_conf);
			$nb_links++;
		}


	echo "<script>$('#link').html('".box_links($arr_links)."'); $('#dwnbtn').html('".$html_dwnbtn."');</script>";
	if (strlen($msg)>0)
		echo "<script>$('#errboxmsg').html('".$msg."'); $('#errbox').css('display','block');</script>";

} while(0);

?>

<br>

<script>
	$("[name='btn-conv2']").removeClass('btn-primary');  $("[name='btn-conv2']").addClass('menu-select');
</script>
