<!DOCTYPE html>
<html lang="en" >

<head>
    <title>Maggot - Metadata Documentation</title>
	<?php echo $HEAD ?>
    <link rel="stylesheet" href="<?php echo $PATH ?>css/doc.css">
</head>

<body>

<body style="zoom: <? echo $ZOOMWP ?>">

<?php echo $header_doc ?>
<br><br>

<section>

<?php

$parser = new ParsedownExtra();
echo preg_replace('/<h2>([^<]+)<\/h2>/', '<a name="\1"></a><h2>\1</h2>',
     preg_replace('/<h3>([^<]+)<\/h3>/', '<a name="\1"></a><h3>\1</h3>',
     str_replace('<p>.</p>','<br>', 
     str_replace('>http',' target="_blank">http',
     str_replace('>@',' target="_blank">',
            $parser->text(file_get_contents($meta_file))
     )))));

?>

</section>

<br/>

<script>
    $("[name='btn-doc']").removeClass('btn-primary');  $("[name='btn-doc']").addClass('menu-select');
</script>
