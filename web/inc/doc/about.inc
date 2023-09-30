<!DOCTYPE html>
<html lang="en" >

<head>
    <title>Maggot - About</title>
	<?php echo $HEAD ?>
    <link rel="stylesheet" href="<?php echo $PATH ?>css/doc.css">
</head>

<body style="zoom: <? echo $ZOOMWP ?>">

<?php echo $header_doc ?>
<br><br>

<section>

<?php
 
$parser = new ParsedownExtra();
echo str_replace('<p>.</p>','<br>', 
     preg_replace('/<a(.*?)href="([^"]+)"(.*?)>/i','<a$1href="$2"$3 target="_blank">',
     str_replace('>@',' target="_blank">',
            $parser->text(file_get_contents($about_file))
     )));

?>

</section>

<br/>

<script>
    $("[name='btn-about']").removeClass('btn-primary');  $("[name='btn-about']").addClass('menu-select');
</script>
