<?php

$ID = genKEY();
$input_url = 'tmp/'.$ID.'.html';
$ID = genKEY();
$md_url = 'tmp/'.$ID.'.md';
$output_url = 'tmp/'.$ID.'.html';

`rm -f tmp/*`; /**/

include ('inc/parsedown/Parsedown.inc');
include ('inc/parsedown/ParsedownExtra.inc');

?>

<!DOCTYPE html>
<html lang="en" >

<head>
	<title>Maggot - Config</title>
	<?php echo $HEAD ?>
</head>

<body style="zoom: <? echo $ZOOMWP ?>">

<?php echo $header_config ?>
<br><br>

<h1>Convert a terminology documentation file : TSV format to Markdown</h1>
<br>

<section>
	<div id="mytest"></div>
	<br/>
	<form id="formconvert" name="formconvert" action="#" autocomplete="off" method="post" enctype="multipart/form-data">
		<h2>Upload a Terminology documentation file</h2>
		<div class="input-group mb-3">
			<div class="input-group-prepend">
				<span class="input-group-text">Select a TSV file</span>
			</div>
			<div class="custom-file">
				<input type="file" class="custom-file-input" name="configdoc" id="configdoc">
				<label id="label" class="custom-file-label" for="configdoc"></label>
			</div>
		</div>
		<input type="submit" class="btn btn-success btn-xs" style="width: 100px;" name="submit" value="Upload" />
		<input type="reset" value="Empty the form" class="btn btn-success btn-xs" style="width: 150px;" onclick="document.getElementById('label').innerHTML=''; document.getElementById('link').innerHTML=''; document.getElementById('download').style.display='none';"/>
		<br><br>
		<div id='link'></div>
		<br>
		<input id="download" type="download" value="Download the Markdown file" class="btn btn-success btn-xs" style="width: 300px; display: none" onclick="downloadFile('<?php echo $PATH.$md_url ?>','doc.md');"/>
		<br>
		<div id="container" class="table-responsive"></div>
	</form>
</section>
<br>

<?php

do {
	// checks if the 'configdoc' file has been submitted
	if( ! isset($_POST['submit']) || ! isset($_FILES["configdoc"]) ) break;

	//if there was an error uploading the file
	if ($_FILES["configdoc"]["error"] > 0) {
	echo "Return Code: " . $_FILES["configdoc"]["error"] . "<br />";
	break;
	}

	$name = $_FILES['configdoc']['name'];
	$ext = strtolower(end(explode('.', $_FILES['configdoc']['name'])));
	$type = $_FILES['configdoc']['type'];
	$tmpName = $_FILES['configdoc']['tmp_name'];
	echo "<script>$('#label').html('".$name."');</script>";

	// Convert TSV to HTML
	$outhtml = tsv2html($tmpName);
	file_put_contents($input_url, $outhtml);

	// Convert TSV to MD
	$outmd = tsv2md($tmpName);
	file_put_contents($md_url, $outmd);

	if (strlen($outmd)>0) {
		$parser = new ParsedownExtra();
		$outhtml = str_replace('<p>.</p>','<br>',
					str_replace('>http',' target="_blank">http',
					str_replace('>@',' target="_blank">',
					$parser->text($outmd)
				)));
		file_put_contents($output_url, $outhtml);
		$arr_links = array();
		$arr_links[0]=array('Input file',$PATH.$input_url,'config_doc.html');
		$arr_links[1]=array('Output file',$PATH.$output_url,'doc_md.html');
		echo "<script>$('#link').html('".box_links($arr_links)."'); $('#download').css('display','block');</script>";
	}

} while(0);

?>

<br>

<script>
    /* Montrer le nom du fichier selectionner pour chargement : https://www.codeply.com/p/LtpNZllird */
    document.querySelector('.custom-file-input').addEventListener('change',function(e){
      var fileName = document.getElementById("configdoc").files[0].name;
      var nextSibling = e.target.nextElementSibling
      nextSibling.innerText = fileName
    });

    $("[name='btn-conv3']").removeClass('btn-primary');  $("[name='btn-conv3']").addClass('menu-select');
</script>
