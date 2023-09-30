<?php

$ID = genKEY();
$json_url = 'tmp/'.$ID.'.json';
$input_url = 'tmp/'.$ID.'.html';
$schema_url = 'tmp/'.$ID.'-schema.json';
$doctmpl_url = 'tmp/'.$ID.'.txt';

?>

<!DOCTYPE html>
<html lang="en" >

<head>
    <title>Maggot - Config</title>
	<?php echo $HEAD ?>

    <script type="text/javascript">
    function copyFiles(key) {
        if (confirm("Do you really want to replace the current JSON files with the newly generated ones?") == true) {
            var req = new XMLHttpRequest();
            req.open("GET", "copy?key="+key, true);
            req.onreadystatechange = function() {
                var divcontainer = document.getElementById("container");
                var divbuttons = document.getElementById("buttons");
                if(req.readyState == 4 && req.status == 200) {
                   divcontainer.innerHTML= req.responseText;
                   document.getElementById('copy').style.display='none';
                }
                if(req.status == 500) {
                   divcontainer.innerHTML = '<p style="color:red">500 Internal Server Error</p><p style="color:orange">Warning: you have to download the JSON files and upload them to the right place yourself.</p><br><br>';
                   document.getElementById('copy').style.display='none';
                }
            }
            req.send();
        };
    }
    </script>

</head>

<body style="zoom: <? echo $ZOOMWP ?>">

<?php echo $header_config ?>
<br><br>

<h1>Convert a terminology definition file : TSV format to JSON</h1>
<br>

<section>
    <div id="mytest"></div>
    <br/>
    <form id="formconvert" name="formconvert" action="#" autocomplete="off" method="post" enctype="multipart/form-data">

        <h2>Upload a Terminology definition file</h2>
        <div class="input-group mb-3">
            <div class="input-group-prepend">
                <span class="input-group-text">Select a TSV file</span>
            </div>
            <div class="custom-file">
                <input type="file" class="custom-file-input" name="configterms" id="configterms">
                <label id="label" class="custom-file-label" for="configterms"></label>
            </div>
        </div>
        <input type="submit" class="btn btn-success btn-xs" style="width: 100px;" name="submit" value="Upload" />
        <input type="reset" value="Empty the form" class="btn btn-success btn-xs" style="width: 150px;" onclick="document.getElementById('label').innerHTML=''; document.getElementById('link').innerHTML=''; document.getElementById('container').innerHTML=''; document.getElementById('copy').style.display='none'; document.getElementById('download').style.display='none';document.getElementById('dwnschema').style.display='none';document.getElementById('dwndoctmpl').style.display='none';document.getElementById('errbox').style.display='none';"/>
        <br><br>
        <div style="padding-bottom: 10px;"><div id="errbox" name="errbox" class="msgbox errbox">
             <div style="margin-left: 2rem; margin-top: 1rem; margin-bottom: 0rem;">
                  <span id="errboxmsg">Error !!!</span></p>
              </div>
        </div></div>
        <div id='link'></div>
		<br>
        <table><tr>
        <td><input id="copy" type="button" value="Replace the current JSON files" class="btn btn-success btn-xs" style="width: 250px; display: none;" onclick="copyFiles('<?php echo $ID ?>');"/></td>
        <td>&nbsp;&nbsp;</td>
        <td><input id="download" type="download" value="Download the JSON file" class="btn btn-success btn-xs" style="width: 200px; display: none" onclick="downloadFile('<?php echo $PATH.$json_url ?>','config_terms.json');"/></td>
        <td>&nbsp;&nbsp;</td>
        <td><input id="dwnschema" type="download" value="Download the JSON Schema file" class="btn btn-success btn-xs" style="width: 270px; display: none" onclick="downloadFile('<?php echo $PATH.$schema_url ?>','maggot-schema.json');"/></td>
        <td>&nbsp;&nbsp;</td>
        <td><input id="dwndoctmpl" type="download" value="Download template for Doc" class="btn btn-success btn-xs" style="width: 270px; display: none" onclick="downloadFile('<?php echo $PATH.$doctmpl_url ?>','config_doc.txt');"/></td>
        </tr></table>
        <br>
        <div id="container" class="table-responsive"></div>
    </form>
</section>
<br>

<?php

do {
    // checks if the 'configterms' file has been submitted
    if( ! isset($_POST['submit']) || ! isset($_FILES["configterms"]) ) break;

    //if there was an error uploading the file
    if ($_FILES["configterms"]["error"] > 0) {
      echo "<script>$('#errboxmsg').html('Return Code: ".$_FILES["configterms"]["error"]."'); $('#errbox').css('display','block');</script>";
      break;
    }

    $name = $_FILES['configterms']['name'];
    $ext = strtolower(end(explode('.', $_FILES['configterms']['name'])));
    $type = $_FILES['configterms']['type'];
    $tmpName = $_FILES['configterms']['tmp_name'];
    echo "<script>document.getElementById('label').innerHTML='$name';</script>";

    // Convert TSV to JSON
    $msg = tsv2json($tmpName, $outjson);
    if (strlen($msg)) {
        echo "<script>$('#errboxmsg').html('".$msg."'); $('#errbox').css('display','block');</script>";
        break;
    }

    // Convert TSV to HTML
    $outhtml = tsv2html($tmpName);
    file_put_contents($input_url, $outhtml);

    // Convert TSV to JSON-Schema
    $outschema = tsv2schema($tmpName, $url_schema);
    file_put_contents($schema_url, $outschema);

    // Convert TSV to a template for documentation
    file_put_contents($doctmpl_url, tsv2doctmpl($tmpName));

    if (strlen($outjson)>0) {
        file_put_contents($json_url, $outjson);
		$arr_links = array();
		$arr_links[0]=array('Input file',$PATH.$input_url,'config_terms.html');
		$arr_links[1]=array('Output file',$PATH.$json_url,'config_terms.json');
		$arr_links[2]=array('JSON-Schema file',$PATH.$schema_url,'maggot-schema.json');
        echo "<script>document.getElementById('link').innerHTML='".box_links($arr_links)."'; document.getElementById('copy').style.display='block'; document.getElementById('download').style.display='block';document.getElementById('dwnschema').style.display='block';document.getElementById('dwndoctmpl').style.display='block';</script>";
    }

} while(0);

?>

<br>

<script>
    /* Montrer le nom du fichier selectionner pour chargement : https://www.codeply.com/p/LtpNZllird */
    document.querySelector('.custom-file-input').addEventListener('change',function(e){
      var fileName = document.getElementById("configterms").files[0].name;
      var nextSibling = e.target.nextElementSibling
      nextSibling.innerText = fileName
    });

    $("[name='btn-conv1']").removeClass('btn-primary');  $("[name='btn-conv1']").addClass('menu-select');
</script>
