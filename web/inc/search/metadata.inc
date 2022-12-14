<?php

$id_metadata='';
$PATH='';

function build_table($section, $arr) {
    global $array_values, $PATH, $filebrowser;
    $myhtml='<table class="table"><thead class="thead-light"><tr>';

    //header
    foreach($arr as $field)
        $myhtml=$myhtml.'<th scope="col">'.get_label($field).'</th>';
    if ($section==='status')
        $myhtml=$myhtml.'<th scope="col">Path on the server</th>';
    $myhtml=$myhtml.'</tr></thead><tbody><tr>';

    //content
    foreach($arr as $field) {
        if ($field==='title' || $field==='description') continue;
        $myhtml=$myhtml.'<td>'.display_array($array_values[$field]).'</td>';
    }
    if ($section==='status') {
        $val=$array_values['chemin'];
        $json_file = str_replace('pgd_data','mnt',$val);
        $json_url = 'file://'.str_replace('pgd_','',$val);
        if (file_exists($json_file) && is_readable($json_file)) {
            $json_content = file_get_contents($json_file);
            $json_url = "tmp/".basename($json_file);
            file_put_contents($json_url, $json_content);
        }
        if ($filebrowser)
           $myhtml=$myhtml.'<td><a href="'.$PATH.'../files/'.basename(dirname($val)).'" target="_blank">'.str_replace('pgd_','',$val).'</a></td>';
        else
           $myhtml=$myhtml.'<td><a href="'.$PATH.$json_url.'" target="_blank">'.str_replace('pgd_','',$val).'</a></td>';
    }
    $myhtml=$myhtml.'</tr></tbody></table>';
    return $myhtml;
}

// id given in the path
if ($_SERVER['PATH_INFO']) {
    $urloptions = explode("/",$_SERVER['PATH_INFO']);
    array_shift($urloptions);
    if (count($urloptions)>0) {
        $id_metadata =  array_shift($urloptions);
        $PATH='../';
    }
}
// id given as a query parameter (GET)
if( isset( $_GET['id'] ) ){
    $id_metadata =  $_GET['id'];
}

?>

<!DOCTYPE html>
<html lang="en" >

<head>
    <meta http-equiv="Content-Type" content="text/html;charset=utf-8" />
    <title>Maggot - Metadata</title>
    <link rel="icon" href="<?php echo $PATH ?>favicon.ico" />

    <!-- CSS -->
    <link rel="stylesheet" href="<?php echo $PATH ?>css/bootstrap.min.css">
    <link rel="stylesheet" href="<?php echo $PATH ?>css/style.css">
    <link rel="stylesheet" href="<?php echo $PATH ?>css/bootstrap-toc.css">

</head>

<body>

<div class="entete">Metadata Management Tool for Dataset</div>
<br>

<div>

<?php

if( strlen($id_metadata) )
{
    // Options affichage
    $options = array('projection'=>[]);
    foreach(array_keys((array)$txtboite) as $key) $options['projection'][$key]=true;
    foreach(array_keys((array)$listboite) as $key) $options['projection'][$key]=true;
    foreach(array_keys((array)$checkboite) as $key) $options['projection'][$key]=true;
    foreach(array_keys((array)$areaboite) as $key) $options['projection'][$key]=true;
    $options['projection']['chemin']=true;

    // Requête pour récupérer l'ensemble des champs de la table metadata pour ce jeu de donnée
    $filter = [ 'title' => $id_metadata ];
    $query = new MongoDB\Driver\Query($filter, $options);
    $cursor_count = $client->executeQuery('pgd-db.metadata', $query)->toArray();
    $count = count($cursor_count);
    $cursor = $client->executeQuery('pgd-db.metadata', $query);

    // Resultats ?
    if( $count < 1 ){
        echo  '<div class="p-3 mb-2 bg-warning text-black">No metadata available for this dataset ...</div>';
    } else {

        // Affichage //
        foreach($cursor as $key => $value) {

            // Get all values
            $array_values = (array)get_object_vars($value);

            // Title
            if (array_key_exists('title',$array_values))
                echo "<h2>".$array_values['title']."</h2><br>";

            // Description of the Dataset
            if (array_key_exists('description',$array_values)) {
                echo '<h3>DESCRIPTION</h3>';
                $newval = preg_replace('"(https?://[^ ]+)"', '<a href="$1" target="_blank">$1</a>', str_replace( ['_NL_'], ['<br>'], $array_values['description']));
                echo "<h2 class='description'>".$newval."</h2><br/>";
            }

            //For each section
            foreach($section['labels'] as $name => $text) {
                if ($name==="definition") continue;
                if ($name==="resources") continue;
                echo '<h3>'.$text.'</h3><div class="table-responsive-sm">';
                $fields = array_keys(array_flip($section[$name]));
                $myhtml=''; $NC=5;
                # Display of fields in groups of $NC on the same line
                for($i=0; $i<count($fields); $i+=$NC) {
                    $arr=[]; $jmax=$i+$NC;
                    for($j=$i; $j<$jmax; $j++) {
                        if ($fields[$j]==='description') { $jmax++; continue; }
                        if ($j<count($fields)) array_push($arr, $fields[$j]);
                    }
                    $myhtml=$myhtml.build_table($name, $arr);
                }
                $myhtml=$myhtml.'</div><br/>';
                echo $myhtml;
            }

            // Check if Resources
            $path=$array_values['chemin'];
            $json_file = str_replace('pgd_data','mnt',$path);
            $json_content = json_decode(file_get_contents($json_file), NULL, 512, JSON_OBJECT_AS_ARRAY);
            if (! array_key_exists('resources',$json_content)) continue;
            $res = $json_content['resources'];
            if (count($res)<1) continue;

            // Resources
            echo '<h3>RESOURCES</h3>';
            $myhtml='<table class="table"><thead class="thead-light">';
            $myhtml=$myhtml.'<tr><th scope="col">Type</th><th scope="col">Description</th><th scope="col">Location</th></tr></thead><tbody>';
            for($i=0; $i<count($res); $i++) {
                 $location=preg_replace('"(https?://[^ ]+)"', '<a href="$1" target="_blank">$1</a>',$res[$i]['location']);
                 $myhtml=$myhtml.'<tr><td>'.$res[$i]['datatype'].'</td><td>'.$res[$i]['description'].'</td><td>'.$location.'</td></tr>';
            }
            $myhtml=$myhtml.'</tbody></table><br/>';
            echo $myhtml;

        }
        echo "</div>";
    }
}
?>

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
