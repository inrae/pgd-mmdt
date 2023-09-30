<!DOCTYPE html>
<html lang="en" >

<head>
    <title>Maggot - Configuration documentation</title>
	<?php echo $HEAD ?>
    <link rel="stylesheet" href="<?php echo $PATH ?>css/doc.css">
</head>

<body style="zoom: <? echo $ZOOMWP ?>">

<?php echo $header_title ?>
<br><br>

<section>

<?php

$parser = new ParsedownExtra();
echo str_replace('<p>.</p>','<br>', 
     str_replace('>http',' target="_blank">http',
     str_replace('>@',' target="_blank">',
     str_replace('.js">','.js" target="_blank">',
            $parser->text(file_get_contents($config_file))
     ))));

?>

</section>

<br/>
