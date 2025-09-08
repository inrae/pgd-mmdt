<!DOCTYPE html>
<html lang="en" >

<head>
    <title>Maggot - Metadata Documentation</title>
	<?= $HEAD ?>
    <link rel="stylesheet" href="<?= $PATH ?>css/doc.css">
</head>

<body>

<body style="zoom: <? echo $ZOOMWP ?>">

<?= $header_doc ?>
<br><br>

<section>
<div id="core" style="inline-size: 92%; overflow-wrap: break-word; text-align: justify;">

<?php

$parser = new ParsedownExtra();
echo preg_replace('/<h2>([^<]+)<\/h2>/', '<a name="\1"></a><h2>\1</h2>',
     preg_replace('/<h3>([^<]+)<\/h3>/', '<a name="\1"></a><h3>\1</h3>',
     str_replace('<p>.</p>','<br>', 
     preg_replace('/<a(.*?)href="([^"]+)"(.*?)>/i','<a$1href="$2"$3 target="_blank">',
     str_replace('>@',' target="_blank">',
            $parser->text(file_get_contents($meta_file))
     )))));
#     str_replace('>http',' target="_blank">http',
?>

</div>
</section>

<br/>

<script>
    $("[name='btn-doc']").removeClass('btn-primary');  $("[name='btn-doc']").addClass('menu-select');
</script>
