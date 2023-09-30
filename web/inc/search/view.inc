<?php

function html_cond($key,$val) {
    $skey = str_replace('_',' ',$key);
    return "<span style=\"color:rgb(47,105,211)\"><b>$skey</b></span><b> &#10153; </b><span style=\"color:rgb(24,114,106)\"><b><i>$val</b></i></span>";
}

function display_request($operator, $arr_query) {
    $op = "&nbsp;&nbsp;<span style=\"color:rgb(223,66,66)\"><b>".strtoupper(str_replace('$','', $operator))."</b></span>&nbsp;&nbsp;";
    $str_query = '<p style="font-size: smaller"><b>&#10100;&nbsp;&nbsp;</b>'.implode($op,$arr_query).'<b>&nbsp;&nbsp;&#10101;</b></p>';
    return $str_query;
}

// Traitement de la recherche
if( isset( $_POST['param'] ) ){

    // Clauses where pour requêtes mongo
    $where_search = array();
    $array_where = array();

    // get arrays by field type
    $array_freefield = [];
    $array_multi = [];
    foreach(array_keys($section_search['labels']) as $key) {
        if (!array_key_exists($key, $section_search)) continue;
        foreach($section_search[$key] as $field)
           if (! array_key_exists($field, $checkboite) && ! array_key_exists($field, $multiboite)) array_push($array_freefield, $field);
    }

    // Décoder le json contenant les données à chercher
    $params = json_decode($_POST['param'], TRUE);

    // Options
    $options = array('projection'=>[], 'sort' => [$params['_field_'] => $params['_sortby_']]);
    foreach($shortview as $key)
        $options['projection'][$key]=true;

    // Parser le json de données saisies
    $arr_query = array();
    foreach($params as $skey => $val) {
        if ($skey == 'operator') {
            $operator = $val;
        }
		if ($key == '_field_' || $key == '_sortby_') continue;
        $key = preg_replace('/-\d+$/','',$skey);
        if (in_array($key, $array_freefield)) { # Champs libres ou balises select
            if( strlen($val) || is_numeric($val) ){
                if ($val !== 'All'){
                    array_push($arr_query, html_cond($key,$val));
                    $filter = new MongoDB\BSON\Regex("$val","i");
                    $array_temp = array("$key" => $filter);
                    array_push($array_where, $array_temp);
                }
            }
        } else {
            # Checkboxes sous la forme 'variable-23' par exemple (.+_\d+)
            if (preg_match('/(\w+)-\d+/', $skey, $matches) == 1) {
                array_push($arr_query, html_cond($key,$val));
                $filter = new MongoDB\BSON\Regex("$val","i");
                $array_temp = array("$matches[1]" => $filter);
                array_push($array_where, $array_temp);
            }
            # Multi-select sous la forme 'variable-sel'
            if (preg_match('/(\w+)-sel/', $skey, $matches) == 1) {
                $fkey=preg_replace('/-sel$/','',$skey);
                $terms = explode(", ", $val);
                if (count($terms)>0 && strlen($terms[0])>0)
                    foreach($terms as $term) {
                        array_push($arr_query, html_cond($fkey,$term));
                        $filter = new MongoDB\BSON\Regex("$term","i");
                        $array_temp = array("$matches[1]" => $filter);
                        array_push($array_where, $array_temp);
                    }
            }
        }
    }

    // if no specific selection was made, then select all
    if (count($array_where)==0) {
        $where_search=array();
        $arr_query = array('all');
    } else { 
        // Final where condition
        $where_search = array(
            "$operator" => $array_where
        );
    }

    // Display the query
    echo display_request($operator, $arr_query);

    // Find all datasets matching the entered criteria
    $query = new MongoDB\Driver\Query($where_search, $options);
    $obj = $client->executeQuery('pgd-db.metadata', $query)->toArray();
    $ret = count($obj);
//echo "<br><pre>"; print_r($obj); echo "</pre><br>";

    // has the serach failed ?
    if ($ret <1  ) {
        echo  '<div class="p-3 mb-2 bg-warning text-black">No datasets available that match your search ...';
    } else {
        // Header
        //echo  '<div class="p-3 mb-2 bg-success text-white">Dataset number : '.$count.'</div>';
        echo '<div class="tableFixHead"><table class="table table-hover">';
        echo '<thead class="thead-light thead-view"><tr>'; // scope="col"
		$array_view = array(); $cnt=0;
        foreach($shortview as $key) {
            //echo'<th>'.get_label($key).'</th>';
			echo'<th><button style="border: none; background-color: #c2d5e9; color: #585894; font-weight: bold;" onclick="sendData(_field_=\''.$key.'\', _sortby_=signsort['.$cnt.']); signsort['.$cnt.']= -signsort['.$cnt.'];">'.get_label($key).'</button></th>';
			array_push($array_view, '-'); # default value
			$cnt++;
		}
        echo '<th>Metadata</th></tr></thead><tr>';

        $countdataset = 0;
        foreach($obj as $key => $value) {
			$cnt=0;
			$arr_out = $array_view;
            foreach($value as $key => $val) {
                if ($key == "_id") { $countdataset++; continue; }
                if ($key == "title") { $idmetadata = $val; }
                if ($key == "description") $val = preg_replace('"(https?://[^ ]+)"', '<a href="$1" target="_blank">$1</a>',preg_replace("/(_NL_)+/",'<br>',$val));
                if (is_array($val)) {
                    if (count($val) > 1) {
                        $val1 = implode("<li>",$val)."</li>";
						$arr_out[$cnt] = '<li>'.$val1;
                    } else {
                        $val1 = implode(" ",$val);
						$arr_out[$cnt] = $val1;
                    }
                } else {
					$arr_out[$cnt] = $val;
                }
				$cnt++;
            }
			foreach($arr_out as $val)
				echo '<td>'.$val.'</td>';

            // Metadata link
            echo "<td><a target=\"_blank\" href=\"metadata/$idmetadata\"><img src=\"img/metadata-icon.png\"></a></td>";
            echo "</tr><tr>";
        }
        echo "</table></div>";
    }
}

?>
