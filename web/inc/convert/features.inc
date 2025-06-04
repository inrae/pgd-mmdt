<?php

# Get the HTML feature form depending on the field type
function get_feature_form ($infos)
{
	// defaut feature values
	$arr1 = get_vocab_array($typeahead=0); array_push($arr1, 'typeahead');
	$arr2 = get_vocab_array($typeahead=1);
	$min = 3;
	$autocompl = $ws = $width = $onto = '';
	$open=0; $rows=6; $cols=30;

	$arr = assocPars($infos[5]);
//echo "<pre>"; print_r($arr); echo "</pre>";

	// Multiselect Type
	$display = 'none';
	$display_ws = 'none';
	$display_min = 'none';
	$display_onto = 'none';
	if ($infos[4]=='multiselect') {
		$arr1 = array_merge($arr1, get_dico_array());
		$autocompl = isset($arr['autocomplete']) ? $arr['autocomplete'] : 'typeahead';
		$ws = isset($arr['ws']) ? $arr['ws'] : 'bioportal';
		$onto = isset($arr['onto']) ? $arr['onto'] : '';
		$min = isset($arr['min']) ? $arr['min'] : $min;
		$display = 'inline-block';
		if ($autocompl !== '') $display_min = 'inline-block';
		if ($autocompl == 'typeahead') $display_ws = $display_onto = 'inline-block';
	}

	$html_features = '
<div id="features_multiselect" style="display: '.$display.'">
<div class="form-group2" style="width:200px">
	<span class="labeldiv">Autocomplete</span>
	<fieldset>
		<select id="multi_autocompl" name="multi_autocompl" class="form-control form-control-sm section" onchange="autocomplChange(\'multi\')">'.get_HTML_options($arr1, $autocompl).'</select>
	</fieldset>
</div>
&nbsp;
<div id="feature_ws" class="form-group2" style="width:200px; display: '.$display_ws.'">
	<span class="labeldiv">WS</span>
	<fieldset>
		<select id="ws" name="ws" class="form-control form-control-sm section">'.get_HTML_options($arr2, $ws).'</select>
	</fieldset>
</div>
&nbsp;
<div id="multi_feature_min" class="form-group2" style="width:50px; display: '.$display_min.'">
	<span class="labeldiv">Min</span>
	<input class="form-control form-control-sm solo" type="text" name="multi_min" id="multi_min" style="width:50px" value="'.$min.'">
</div>
<div id="feature_onto" class="form-group2" style="width:575px; display: '.$display_onto.'">
	<span class="labeldiv">Ontologies</span>
	<input class="form-control form-control-sm solo" type="text" name="onto" id="onto" style="width:575px" placeholder="ontology list separated by a colon" value="'.$onto.'">
</div>
</div>';

	// Textbox Type
	$display = 'none';
	$display_min = 'none';
	if ($infos[4]=='textbox') {
		$autocompl = isset($arr['autocomplete']) ? $arr['autocomplete'] : '';
		$min = isset($arr['min']) ? $arr['min'] : 1;
		$width = isset($arr['width']) ? $arr['width'] : $width;
		$display = 'inline-block';
		if ($autocompl !== '') $display_min = 'inline-block';
	}

	$html_features .= '
<div id="features_textbox" style="display: '.$display.'">
<div class="form-group2" style="width:200px">
	<span class="labeldiv">Autocomplete</span>
	<fieldset>
		<select id="text_autocompl" name="text_autocompl" class="form-control form-control-sm section" onchange="autocomplChange(\'text\')">'.get_HTML_options(get_vocab_array($typeahead=0), $autocompl).'</select>
	</fieldset>
</div>
&nbsp;
<div id="text_feature_min" class="form-group2" style="width:50px; display: '.$display_min.'">
	<span class="labeldiv">Min</span>
	<input class="form-control form-control-sm solo" type="text" name="text_min" id="text_min" style="width:50px" value="'.$min.'">
</div>
&nbsp;
<div class="form-group2" style="width:150px">
	<span class="labeldiv">Width</span>
	<input class="form-control form-control-sm solo" type="text" name="text_width" id="text_width" style="text_width:150px" value="'.$width.'">
</div>
</div>';

	// Areabox Type
	$display = 'none';
	if ($infos[4]=='areabox') {
		$rows = isset($arr['rows']) ? $arr['rows'] : $rows;
		$cols = isset($arr['cols']) ? $arr['cols'] : $cols;
		$display = 'inline-block';
	}

	$html_features .= '
<div id="features_areabox" style="display: '.$display.'">
<div class="form-group2" style="width:50px">
	<span class="labeldiv">Rows</span>
	<input class="form-control form-control-sm solo" type="text" name="rows" id="cols" style="width:50px" value="'.$rows.'">
</div>
&nbsp;
<div class="form-group2" style="width:50px">
	<span class="labeldiv">Cols</span>
	<input class="form-control form-control-sm solo" type="text" name="cols" id="cols" style="width:50px" value="'.$cols.'">
</div>
</div>';

	// Dropbox Type
	$display = 'none';
	if ($infos[4]=='dropbox') {
		$width = isset($arr['width']) ? $arr['width'] : $width;
		$display = 'inline-block';
	}

	$html_features .= '
<div id="features_dropbox" style="display: '.$display.'">
<div class="form-group2" style="width:50px">
	<span class="labeldiv">Width</span>
	<input class="form-control form-control-sm solo" type="text" name="drop_width" id="drop_width" style="width:150px" value="'.$width.'">
</div>
</div>';

	// Datebox Type
	$display = 'none';
	if ($infos[4]=='datebox') {
		$width = isset($arr['width']) ? $arr['width'] : $width;
		$display = 'inline-block';
	}

	$html_features .= '
<div id="features_datebox" style="display: '.$display.'">
<div class="form-group2" style="width:50px">
	<span class="labeldiv">Width</span>
	<input class="form-control form-control-sm solo" type="text" name="date_width" id="date_width" style="width:150px" value="'.$width.'">
</div>
</div>';

	// Checkbox Type
	$display = 'none';
	if ($infos[4]=='checkbox') {
		$open = isset($arr['open']) ? $arr['open'] : $open;
		$display = 'inline-block';
	}

	$html_features .= '
<div id="features_checkbox" style="display: '.$display.'">
<div class="form-group2" style="width:50px">
	<span class="labeldiv">Open</span>
	<input class="form-control form-control-sm solo" type="text" name="open" id="open" style="width:150px" value="'.$open.'">
</div>
</div>';

	return $html_features;
}

# Get the features line depending on the field type
function get_features_line ()
{
	switch ($_POST['type']) {
	case 'multiselect' :
		$autocompl = $_POST['multi_autocompl'];
		$features = 'autocomplete='.$autocompl;
		if ($autocompl == 'typeahead')
			$features .= ',ws='.$_POST['ws'].',onto='.$_POST['onto'];
		$features .= ',min='.$_POST['multi_min'];
		break;
	case 'textbox' :
		$features = '';
		$autocompl = $_POST['text_autocompl'];
		$width = $_POST['text_width'];
		if (strlen($autocompl))
			$features .= 'autocomplete='.$autocompl.',min='.$_POST['text_min'];
		if (strlen($autocompl) && strlen($width))
			$features .= ',';
		if (strlen($width))
			$features .= 'width='.$width;
		break;
	case 'areabox' :
		$features = 'rows='.$_POST['rows'].',cols='.$_POST['cols'];
		break;
	case 'checkbox' :
		$features = 'open='.$_POST['open'];
		break;
	case 'dropbox' :
		$features = '';
		$width = $_POST['drop_width'];
		if (strlen($width))
			$features .= 'width='.$width;
		break;
	case 'datebox' :
		$features = '';
		$width = $_POST['date_width'];
		if (strlen($width))
			$features .= 'width='.$width;
		break;
	}
	return $features;
}

?>
