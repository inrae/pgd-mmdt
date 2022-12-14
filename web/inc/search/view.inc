<?php

function html_cond($key,$val) {
    $skey = str_replace('_',' ',$key);
    return "<span style=\"color:rgb(47,105,211)\"><b>$skey</b></span><b> &#10153; </b><span style=\"color:rgb(24,114,106)\"><b><i>$val</b></i></span>";
}

function display_request($operator, $arr_query) {
    $op = "&nbsp;&nbsp;<span style=\"color:rgb(223,66,66)\"><b>".strtoupper(str_replace('$','', $operator))."</b></span>&nbsp;&nbsp;";
    $str_query = '<p style="font-size: smaller"><b>&#10100;&nbsp;&nbsp;</b>'.implode($op,$arr_query).'<b>&nbsp;&nbsp;&#10101;</b></p>';
    //$label = '<p style="font-size: smaller; color:rgb(24,114,106);"><b>Your request:&nbsp;&nbsp;&nbsp;</b></p>';
    //return '<br><table><tr><td>'.$label.'</td><td>'.$str_query.'</td></tr></table><br>';
    return '<br>'.$str_query.'<br>';
}

// Traitement de la recherche
if( isset( $_POST['param'] ) ){

    // Clauses where pour requêtes mongo
    $where_search = array();
    $array_where = array();
    $array_where_all = array();

    // get arrays by field type
    $array_freefield = [];
    $array_multi = [];
    foreach(array_keys($section_search['labels']) as $key) {
        if (!array_key_exists($key, $section_search)) continue;
        foreach($section_search[$key] as $field)
           if (array_key_exists($field, $checkboite)) array_push($array_multi, $field);
           else                                       array_push($array_freefield, $field);
    }

    // Décoder le json contenant les données à chercher
    $params = json_decode($_POST['param'], TRUE);

    // Options
    $options = array('projection'=>[]);
    foreach($shortview as $key)
        $options['projection'][$key]=true;

    // Parser le json de données saisies
    $arr_query = array();
    foreach($params as $skey => $val) {
        if ($skey == 'operator') {
            $operator = $val;
        }
        $key = preg_replace('/-\d+$/','',$skey);
        if (in_array($key, $array_freefield)) { # Champs libres ou balises select
            if( strlen($val) || is_numeric($val) ){
                if ($val == 'All'){
                    if (array_key_exists($key, $listdico)) {
                        $array_temp = array('$or' => [] );
                        foreach($listdico[$key] as $field) array_push( $array_temp['$or'], array($key => $field) );
                        array_push($array_where_all, $array_temp);
                    } else {
                        $filter = new MongoDB\BSON\Regex("$val","i");
                        $array_temp = array("$key" => $filter);
                        array_push($array_where_all, $array_temp);
                    }
                } else {
                    array_push($arr_query, html_cond($key,$val));
                    $filter = new MongoDB\BSON\Regex("$val","i");
                    $array_temp = array("$key" => $filter);
                    array_push($array_where, $array_temp);
                }
            }
        } else { # Checkboxes sous la forme tag_annee-23 par exemple (.+_\d+)
            if (preg_match('/(\w+)-\d+/', $skey, $matches) == 1) {
                array_push($arr_query, html_cond($key,$val));
                $filter = new MongoDB\BSON\Regex("$val","i");
                $array_temp = array("$matches[1]" => $filter);
                array_push($array_where, $array_temp);
            }
        }
    }

    // if no specific selection was made, then select all
    if (count($array_where)==0) {
        $array_where = $array_where_all;
        $arr_query = array('all');
    }
    // Clause where finale
    $where_search = array(
        "$operator" => $array_where
    );

    // Display the query
    echo display_request($operator, $arr_query);

    // Rechercher tous les jeux de données répondant aux critères saisis
    $query = new MongoDB\Driver\Query($where_search, $options);
    $cursor_count = $client->executeQuery('pgd-db.metadata', $query)->toArray();
    $count = count($cursor_count);
    $cursor = $client->executeQuery('pgd-db.metadata', $query);

    // La recherche renvoie-t-elle des résultats ?
    if( $count < 1 ){
        echo  '<div class="p-3 mb-2 bg-warning text-black">No datasets available that match your search ...';
    } else {
        // Entête
        echo  '<div class="p-3 mb-2 bg-success text-white">Dataset number : '.$count.'</div>';
        echo '<table class="table table-hover">';
        echo '<thead class="thead-light thead-view"><tr><th></th>'; // scope="col"
        foreach($shortview as $key)
            echo'<th>'.get_label($key).'</th>';
        echo '<th>Metadata</th></tr></thead><tr>';

        $countdataset = 1;
        foreach($cursor as $key => $value) {
            foreach($value as $key => $val) {
                if ($key == "_id") { echo  "<td class=\"st\">".$countdataset."</td>"; $countdataset++; continue; }
                if ($key == "title") { $idmetadata = $val; }
                if ($key == "description") $val = preg_replace('"(https?://[^ ]+)"', '<a href="$1" target="_blank">$1</a>',preg_replace("/(_NL_)+/",'<br>',$val));
                if (is_array($val)) {
                    if (count($val) > 1) {
                        $val1 = implode("<li>",$val)."</li>";
                        echo "<td><li>".$val1."</td>";
                    } else {
                        $val1 = implode(" ",$val);
                        echo "<td>".$val1."</td>";
                    }
                } else {
                    echo "<td>".$val."</td>";
                }
            }

            // Acces metadonnee
            echo "<td><a target=\"_blank\" href=\"metadata/$idmetadata\"><img src=\"img/metadata-icon.png\"></a></td>";
            echo "</tr><tr>";
        }
        echo "</table>";
    }
}

?>
