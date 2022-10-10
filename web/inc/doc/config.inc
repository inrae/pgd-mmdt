<!DOCTYPE html>
<html lang="en" >

<head>
    <meta http-equiv="Content-Type" content="text/html;charset=utf-8" />
    <title>Indexator - Configuration documentation</title>

    <!-- CSS -->
    <link rel="stylesheet" href="<?php echo $PATH ?>css/bootstrap.min.css">
    <link rel="stylesheet" href="<?php echo $PATH ?>css/style.css">
    <link rel="stylesheet" href="<?php echo $PATH ?>css/doc.css">
</head>

<body>

<div class="entete">Metadata Management Tool for Dataset - Configuration documentation</div>
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
<hr>

<footer>
   &copy; INRAE 2022&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
   <img src="<?php echo $PATH ?>img/logo_inrae.svg">
</footer>

</body>
</html>
