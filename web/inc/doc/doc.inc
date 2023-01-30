<!DOCTYPE html>
<html lang="en" >

<head>
    <meta http-equiv="Content-Type" content="text/html;charset=utf-8" />
    <title>Maggot - Metadata Documentation</title>
    <link rel="icon" href="<?php echo $PATH ?>favicon.ico" />

    <!-- CSS -->
    <link rel="stylesheet" href="<?php echo $PATH ?>css/bootstrap.min.css">
    <link rel="stylesheet" href="<?php echo $PATH ?>css/style.css">
    <link rel="stylesheet" href="<?php echo $PATH ?>css/doc.css">
    <!-- JS -->
    <script src="<?php echo $PATH ?>js/jquery.min.js"></script>
</head>

<body>

<div class="entete">Metadata Management Tool for Dataset - Documentation</div>
<br>
<div class="left-div">
    <div class="center-div">
        <div class="btn-group btn-group-lg" role="group">
          <a href="<?php echo $PATH ?>doc/about" class="search_data"><button type="button" class="btn btn-outline-secondary mr-2" data-toggle="button" aria-pressed="false" autocomplete="off" name="btn-about">Interface</button></a>
          <a href="<?php echo $PATH ?>doc/meta" class="search_data"><button type="button" class="btn btn-outline-secondary mr-2" data-toggle="button" aria-pressed="false" autocomplete="off" name="btn-doc">Metadata</button></a>
       </div>
    </div>
</div>
<br>
<section>

<?php

$parser = new ParsedownExtra();
echo preg_replace('/<h2>([^<]+)<\/h2>/', '<a name="\1"></a><h2>\1</h2>',
     str_replace('<p>.</p>','<br>', 
     str_replace('>http',' target="_blank">http',
     str_replace('>@',' target="_blank">',
            $parser->text(file_get_contents($meta_file))
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

<script>
    $("[name='btn-doc']").css({'color': '#fff', 'background-color': '#6c757d', 'border-color': '#6c757d'})
</script>
</body>
</html>
