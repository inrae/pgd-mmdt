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


function searchByKeywords(array $keywords, $operator)
{
	// Make sure keywords are lowercase for case-insensitive comparison
	$keywords = array_map('mb_strtolower', $keywords);

	// Browse all the documents in the collection
	$obj = dbQueryToArray();
	$results = [];
	foreach($obj as $key => $value) {
		$title = '';
		$keymatch = [];
		foreach($value as $key => $val) {
			if (in_array($key, array("chemin", "profile", "_id"))) continue;
			if ($key == "title") $title = $val;
			if (is_string($val)) {
				$valueLower = mb_strtolower($val);
				foreach ($keywords as $keyword)
					if (mb_strpos($valueLower, $keyword) !== false)
						$keymatch[$keyword]=1;
			} elseif (is_array($val)) {
				foreach ($val as $item)
					if (is_string($item)) {
						$itemLower = mb_strtolower($item);
						foreach ($keywords as $keyword)
							if (mb_strpos($itemLower, $keyword) !== false)
								$keymatch[$keyword]=1;
					}
			}
		}
		if ( ($operator==='or' && array_sum($keymatch)>0) || ($operator==='and' && array_sum($keymatch)==count($keywords)) )
			$results[$title] = array_sum($keymatch);
	}
	return $results;
}


// Empty result by default
$obj = NULL;


// === Simple search ===
if( isset( $_POST['query'] ) )
{
	$sendType = 2;
	$operator = isset($_POST['operator']) ? $_POST['operator'] : 'or' ;
	$keywords = explode(' ', $_POST['query']);
	$results = searchByKeywords($keywords, $operator);

	// Builds then executes query
	$titles = array_keys($results);
	$filter = ['title' => ['$in' => $titles]];
	$options = array('projection'=>[], 'sort' => ['title' => 1]);
	foreach($shortview as $key) $options['projection'][$key]=true;
	$options['projection']['chemin']=true;
	$obj = dbQueryToArray($filter, $options);
}


// === Advanced search  ===
if( isset( $_POST['param'] ) )
{
	$sendType = 1;

	// Where clauses for Mongo queries
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

	// Decode the json containing the data to search for
	$params = json_decode($_POST['param'], TRUE);

	// Options
	$options = array('projection'=>[], 'sort' => [$params['_field_'] => $params['_sortby_']]);
	foreach($shortview as $key)
		$options['projection'][$key]=true;

	// Parse the json of the request data
	$arr_query = array();
	foreach($params as $skey => $val) {
		if ($skey == 'operator') {
			$operator = $val;
		}
		if ($key == '_field_' || $key == '_sortby_') continue;
		$key = preg_replace('/-\d+$/','',$skey);
		if (in_array($key, $array_freefield)) { # Free fields or select tags
			if( strlen($val) || is_numeric($val) ){
				if ($val !== 'All'){
					array_push($arr_query, html_cond($key,$val));
					$filter = new MongoDB\BSON\Regex("$val","i");
					$array_temp = array("$key" => $filter);
					array_push($array_where, $array_temp);
				}
			}
		} else {
			# Checkboxes in the form 'variable-23' for example (.+_\d+)
			if (preg_match('/(\w+)-\d+/', $skey, $matches) == 1) {
				array_push($arr_query, html_cond($key,$val));
				$filter = new MongoDB\BSON\Regex("$val","i");
				$array_temp = array("$matches[1]" => $filter);
				array_push($array_where, $array_temp);
			}
			# Multi-select in the form 'variable-sel'
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
	if ($sendType==1)
		echo display_request($operator, $arr_query);

	// Find all datasets matching the entered criteria
	$options['projection']['chemin']=true;
	$obj = dbQueryToArray($where_search, $options);
}


// === Display the result if any ===
if (!is_null($obj))
{
//echo "<br><pre>"; print_r($obj); echo "</pre><br>";

	// has the serach failed ?
	$ret = count($obj);
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
			echo'<th><button style="border: none; background-color: #c2d5e9; color: #585894; font-weight: bold;" onclick="sendData(type='.$sendType.',_field_=\''.$key.'\', _sortby_=signsort['.$cnt.']); signsort['.$cnt.']= -signsort['.$cnt.'];">'.get_label($key).'</button></th>';
			array_push($array_view, '-'); # default value
			$cnt++;
		}
		echo '<th>Metadata</th></tr></thead><tr>';

		$countdataset = 0;
		foreach($obj as $key => $value) {
			$arr = (array)($value);
			if (is_accessible ($arr)) continue;
			$cnt=0;
			$arr_out = $array_view;
			foreach($value as $key => $val) {
				if ($key == "chemin") { continue; }
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
