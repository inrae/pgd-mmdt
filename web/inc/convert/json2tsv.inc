<?php

$ID = genKEY();
$input_url = $PATH.'json/config_terms.json';
$schema_url = $PATH.'json/pgd-mmdt-schema.json';
$output_url = 'tmp/'.$ID.'.txt';
$output_html = 'tmp/'.$ID.'.html';

?>

<!DOCTYPE html>
<html lang="fr" >

<head>
    <meta http-equiv="Content-Type" content="text/html;charset=utf-8" />
    <title>Maggot - Config</title>
    <link rel="icon" href="<?php echo $PATH ?>favicon.ico" />

    <!-- CSS -->
    <link rel="stylesheet" href="<?php echo $PATH ?>css/bootstrap.min.css">
    <link rel="stylesheet" href="<?php echo $PATH ?>css/style.css">

    <!-- JS -->
    <script src="<?php echo $PATH ?>js/jquery.min.js"></script>

    <script type="text/javascript">
    function downloadFile(urlToSend,$filename) {
        var req = new XMLHttpRequest();
        req.open("GET", urlToSend, true);
        req.responseType = "blob";
        req.onload = function (event) {
            var blob = req.response;
            var link=document.createElement('a');
            link.href=window.URL.createObjectURL(blob);
            link.download=$filename;
            link.click();
        };
        req.send();
    }    
    </script>

</head>

<body>

<div class="entete">Metadata Management Tool for Dataset - Configuration</div>
<br>
<div class="left-div">
    <div class="center-div">
        <div class="btn-group btn-group-lg" role="group">
          <a href="<?php echo $PATH ?>config" class="search_data"><button type="button" class="btn btn-outline-secondary mr-2" data-toggle="button" aria-pressed="false" autocomplete="off" name="btn-conv1">TSV to JSON</button></a>
          <a href="<?php echo $PATH ?>config/doc" class="search_data"><button type="button" class="btn btn-outline-secondary mr-2" data-toggle="button" aria-pressed="false" autocomplete="off" name="btn-conv3">TSV to DOC</button></a>
          <a href="<?php echo $PATH ?>config/current" class="search_data"><button type="button" class="btn btn-outline-secondary mr-2" data-toggle="button" aria-pressed="false" autocomplete="off" name="btn-conv2">JSON to TSV</button></a>
          <a href="<?php echo $PATH ?>doc/config" class="search_data" target="_blank"><button type="button" class="btn btn-outline-info mr-2" data-toggle="button" aria-pressed="false" autocomplete="off">Documentation</button></a>
        </div>
    </div>
</div>
<br><br>
<h1>Convert the current terminology definition file : JSON format to TSV format</h1>
<br>

<section>
    <br/>
    <form id="formconvert" name="formconvert" action="#" autocomplete="off" method="post" enctype="multipart/form-data">
        <br><hr>
        <div id='link'><br/><br/></div>
        <hr>
       <input id="download" type="download" value="Download the TSV file" class="btn btn-success btn-xs" style="width: 200px; display: none" onclick="downloadFile('<?php echo $PATH.$output_url ?>','config_terms.txt');"/>
    </form>
</section>
<br>

<?php
    //echo "<br>-----<br><pre>".json2tsv()."</pre><br>-----<br>";
    file_put_contents($output_url, json2tsv());

    // Convert TSV to HTML
    if (file_exists($output_url))
        file_put_contents($output_html, tsv2html($output_url));

        $html_input = '<br/>Input file : <a href="'.$input_url.'" target="_blank">config_terms.json.</a><br/>';
        $html_output = '<br/>Output file : <a href="'.$PATH.$output_html.'" target="_blank">config_terms.txt.</a><br/>';
        $html_schema = '<br/>JSON-Schema file : <a href="'.$schema_url.'" target="_blank">pgd-mmdt-schema.json.</a><br/>';
        echo "<script>document.getElementById('link').innerHTML='".$html_input.$html_output.$html_schema."'; document.getElementById('download').style.display='block';</script>";

?>

<br><br>
<hr>
<footer>
   <table><tr>
   <td><img src="<?php echo $PATH ?>docs/logo.png"></td>
   <td style="text-align: bottom;">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</td>
   <td><img src="<?php echo $PATH ?>img/logo_inrae.svg"></td>
   <td style="vertical-align: middle;">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&copy; INRAE 2022&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</td>
   </tr></table>
</footer>

<script>
    $("[name='btn-conv2']").css({'color': '#fff', 'background-color': '#6c757d', 'border-color': '#6c757d'})
</script>

</script>

</body>
</html>
