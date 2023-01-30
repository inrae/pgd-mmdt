<!DOCTYPE html>
<html lang="en" >

<head>
    <meta http-equiv="Content-Type" content="text/html;charset=utf-8" />
    <title>Maggot - Configuration documentation</title>
    <link rel="icon" href="<?php echo $PATH ?>favicon.ico" />

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
   <table><tr>
   <td><img src="<?php echo $PATH ?>docs/logo.png"></td>
   <td style="text-align: bottom;">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</td>
   <td><img src="<?php echo $PATH ?>img/logo_inrae.svg"></td>
   <td style="vertical-align: middle;">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&copy; INRAE 2022&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</td>
   </tr></table>
</footer>

</body>
</html>
