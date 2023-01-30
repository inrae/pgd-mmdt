<?php

// Load 'js/param.json' into the $options array
$options = json_decode(file_get_contents("./json/options.json"), NULL, 512, JSON_OBJECT_AS_ARRAY);

// Get the different dictionnaries
$section=$options['sections'];
$section_search=$options['sections_search'];
$shortview=$options['shortview'];
$checkboite=$options['checkboite'];
$listboite=$options['listboite'];
$txtboite=$options['txtboite'];
$areaboite=$options['areaboite'];
$listdico=$options['listdico'];
$chkbxdico=$options['chkbxdico'];

/********************************************************
* genKEY
********************************************************/
# Generate a KEY (30  alpha-num. char. )
function genKEY()
{
    return md5(uniqid(rand(),true));
}

// Get the html code for $val
function display_array ($val)
{
    if (is_array($val)) {
        if (count($val) > 1) {
            $val_display = "<ul><li>".implode('</li><li>', $val)."</li></ul>";
        } else {
            $val_display = implode(" ",$val);
            if (empty($val_display)) { $val_display = '-'; }
        }
    } else {
        $val_display = preg_replace('"(https?://[^ ]+)"', '<a href="$1" target="_blank">$1</a>',$val);
        if (empty($val_display)) { $val_display = '-'; }
    }
    return $val_display;
}

// Get the label corresponding to the field $field
function get_label($field)
{
    global $checkboite, $txtboite, $listboite, $areaboite;
    if (array_key_exists($field, (array)$checkboite)) $label=$checkboite[$field]['titre'];
    else if (array_key_exists($field, (array)$listboite)) $label=$listboite[$field]['titre'];
    else if (array_key_exists($field, (array)$txtboite)) $label=$txtboite[$field]['titre'];
    else if (array_key_exists($field, (array)$areaboite)) $label=$areaboite[$field]['titre'];
    else $label='-';
    return $label;
}

// Get the features $arr_names corresponding to the field $field in array $typeboite
function get_features($field, $arr_names, $typeboite)
{
    $arr_features=array();
    foreach( $arr_names as $key ) 
        if (array_key_exists($key, (array)$typeboite[$field])) array_push($arr_features, $key.'='.$typeboite[$field][$key]);
    $features = join(',', $arr_features);
    return $features;
}

// JSON to TSV : Convert the current terminology definition file
function json2tsv()
{
    global $section, $section_search, $shortview, $checkboite, $txtboite, $listboite, $areaboite, $chkbxdico, $listdico;

    $headers = "Field\tSection\tSearch\tShortView\tType\tfeatures\tLabel\tPredefined terms\n";
    $content = "";

    // for each section
    foreach( array_keys((array)$section['labels']) as $sec ) {
        // for each field in the section
        foreach( (array)$section[$sec] as $field ) {
            if ($field==='resources') continue;
            $search="N"; $view=''; $type='textbox'; $features=''; $label=''; $terms='';
            if (in_array($field, (array)$section_search[$sec])) $search="Y";
            if (in_array($field, (array)$shortview)) $view=array_search($field, (array)$shortview, TRUE)+1;
            $label = get_label($field);
            if (array_key_exists($field, (array)$checkboite)) {
                $features = get_features($field, array('autocomplete'), $checkboite);
                if (strlen($features)) $features .= ','; 
                $type='checkbox';
                if ($checkboite[$field]['open']) { $features .= 'open=1'; } else { $features .= 'open=0'; }
                $terms = join(',', $chkbxdico[$field]);
            }
            else if (array_key_exists($field, (array)$listboite)) {
                $type='dropbox';
                $terms = join(',', $listdico[$field]);
                $features = get_features($field, array('width'), $listboite);
            }
            else if (array_key_exists($field, (array)$txtboite)) {
                $type='textbox';
                $features = get_features($field, array('autocomplete', 'width'), $txtboite);
            }
            else if (array_key_exists($field, (array)$areaboite)) {
                $type='areabox';
                $features = get_features($field, array('rows', 'cols'), $areaboite);
            }
            else $type='unknown';
            $content .= "$field\t$sec\t$search\t$view\t$type\t$features\t$label\t$terms\n";
        }
    }
    if (array_key_exists('resources', (array)$listdico))
        $content .= "resources\tresources\tN\t\t\t\tResources\t".implode(',',$listdico['resources'])."\n";
    return $headers.$content;
}

// TSV to JSON : Convert a terminology definition file
function tsv2json($filename)
{
    $outjson='';
    do {
        $handle = fopen($filename, "r");
        if (! $handle) break;
    
        $section=[]; $section['labels']=[];
        $section_search=[]; $section_search['labels']=[];
        $shortview=[]; $orderview=[];
        $txtboite=[];
        $listboite=[];
        $checkboite=[];
        $areaboite=[];
        $checkdico=[];
        $listdico=[];

        // Header
        if (! fgets($handle)) break;

        // Content
        while (($line = fgets($handle)) !== false) {
//echo "$line<br>";
            $val = explode("\t", $line);
            // section
            if ( ! array_key_exists($val[1], $section['labels']) ) {
                $section['labels'][$val[1]] = strtoupper($val[1]);
                $section[$val[1]] = [];
            }
            if ( ! array_key_exists($val[0], $section[$val[1]]) )
                array_push( $section[$val[1]], $val[0] );
            // section_search
            if ( $val[2]=='Y' && ! array_key_exists($val[1], $section_search['labels']) ) {
                $section_search['labels'][$val[1]] = strtoupper($val[1]);
                $section_search[$val[1]] = [];
            }
            if ( $val[2]=='Y' && ! array_key_exists($val[0], $section_search[$val[1]]) )
                array_push( $section_search[$val[1]], $val[0] );
            // shortview
            if (is_numeric($val[3]) && ! in_array($val[0], $shortview) ) {
                array_push( $orderview, intval($val[3]) );
                array_push( $shortview, $val[0] );
            }
            //checkboite
            if ( $val[4]=='checkbox' && ! array_key_exists($val[0], $checkboite) ) {
                $checkboite[$val[0]]=[];
                $checkboite[$val[0]]['titre']=utf8_encode($val[6]);
                foreach ( explode(',',$val[5]) as $value )
                    if (strlen($value)) {
                        $arr = explode('=',$value);
                        if ($arr[1]==0 ) { $arr[1]=FALSE; }
                        if ($arr[1]==1 ) { $arr[1]=TRUE; }
                        $checkboite[$val[0]][$arr[0]]=$arr[1];
                    }
            }
            //listboite
            if ( $val[4]=='dropbox' && ! array_key_exists($val[0], $listboite) ) {
                $listboite[$val[0]]=[];
                $listboite[$val[0]]['titre']=utf8_encode($val[6]);
                foreach ( explode(',',$val[5]) as $value )
                    if (strlen($value)) {
                        $arr = explode('=',$value);
                        $listboite[$val[0]][$arr[0]]=$arr[1];
                    }
            }
            //txtboite
            if ( $val[4]=='textbox' && ! array_key_exists($val[0], $txtboite) ) {
                $txtboite[$val[0]]=[];
                $txtboite[$val[0]]['titre']=utf8_encode($val[6]);
                foreach ( explode(',',$val[5]) as $value )
                    if (strlen($value)) {
                        $arr = explode('=',$value);
                        $txtboite[$val[0]][$arr[0]]=$arr[1];
                    }
            }
            //areaboite
            if ( $val[4]=='areabox' && ! array_key_exists($val[0], $areaboite) ) {
                $areaboite[$val[0]]=[];
                $areaboite[$val[0]]['titre']=utf8_encode($val[6]);
                foreach ( explode(',',$val[5]) as $value )
                    if (strlen($value)) {
                        $arr = explode('=',$value);
                        $areaboite[$val[0]][$arr[0]]=$arr[1];
                    }
            }
            //checkdico
            if ( $val[4]=='checkbox' && ! array_key_exists($val[0], $checkdico) ) {
                $checkdico[$val[0]]= [];
                $str_dico = str_replace("\n", '', str_replace("\r\n", '', preg_replace('/, ?/','@',utf8_encode($val[7]))));
                foreach ( explode('@',$str_dico) as $value ) array_push( $checkdico[$val[0]], $value );
            }
            //listdico && resources
            if ( ($val[4]=='dropbox' || $val[0]=='resources') && ! array_key_exists($val[0], $listdico) ) {
                $listdico[$val[0]]=[];
                $str_dico = str_replace("\n", '', str_replace("\r\n", '', preg_replace('/, ?/','@',utf8_encode($val[7]))));
                foreach ( explode('@',$str_dico) as $value ) array_push( $listdico[$val[0]], $value );
            }
        }
        fclose($handle);

        array_multisort($orderview, SORT_NUMERIC, SORT_ASC, $shortview);

        $config_terms=array( 'sections'=>$section, 'sections_search'=>$section_search, 'shortview'=>$shortview,
                             'checkboite'=>$checkboite, 'listboite'=>$listboite, 'txtboite'=>$txtboite, 'areaboite'=>$areaboite,
                             'chkbxdico'=>$checkdico, 'listdico'=>$listdico );

        $outjson = json_encode($config_terms, JSON_UNESCAPED_UNICODE);
        if (strlen($outjson)==0) {
             echo '<br><font color="red">ERROR: Encoding to JSON format failed.</font><br>';
        }
    
    }  while(0);
    return $outjson;
}

// TSV to JSON-Schema : Convert a terminology definition file
function tsv2schema($filename, $url_schema)
{
    $outschema ='{"$schema": "http://json-schema.org/draft-07/schema#", "$id": "'.$url_schema.'",';
    $outschema.='"title": "pgd-mmdt-package","description": "Description of the pgd-mmdt JSON schema", "type": "object"';

    $profile = '"profile": { "title": "pgd-mmdt-package", "description": "JSON schema for the pgd-mmdt tool", "type": "string" }';
    $required = '"required": [ "profile", "title", "description" ]';
    $def_array = '"def_array": { "type": "array", "minItems": 0, "uniqueItems": true, "items": { "type": "string" } }';
    $def_resource = '"resource": {"title":"Resource","type": "object","properties": {"datatype": {"title": "Data type","type": "string"},"description": {"title": "Data description","type": "string"},"location": {"title": "Data location","type": "string"}}}';
    $def_resources = '"resources": {"type": "array","minItems": 0,"items": {"$ref": "#/definitions/resource"}}';

    $definitions = [];
    $properties = [];

    array_push($definitions, $def_array, $def_resource, $def_resources);
    array_push($properties, $profile);

    do {

        $handle = fopen($filename, "r");
        if (! $handle) break;

        // Header
        if (! fgets($handle)) break;

        // Content
        while (($line = fgets($handle)) !== false) {
            $val = explode("\t", $line);
            // resources
            if ( $val[0]=='resources' ) {
                array_push($properties, '"'.$val[0].'": { "title" : "'.$val[6].'", "$ref": "#/definitions/resources" }');
            // checkboite
            } else if ( $val[4]=='checkbox' ) {
                array_push($properties, '"'.$val[0].'": { "title" : "'.$val[6].'", "$ref": "#/definitions/def_array" }');
            } else { 
            //listboite, txtboite, areaboite
                array_push($properties, '"'.$val[0].'": { "title" : "'.$val[6].'", "type": "string" }');
            }
        }
        fclose($handle);
        $outschema .= ', "definitions": {'.join(",", $definitions).'}, "properties": {'.join(",", $properties).'},'.$required;
    
    }  while(0);

    $outschema .='}';

    return $outschema;
}

// TSV to Template Doc  : Convert a terminology definition file to a template file for terminology documentation
function tsv2doctmpl($filename)
{
    $doctmpl="Type\tName\tDescription\n";
    $section='';
    do {
        $handle = fopen($filename, "r");
        if (! $handle) break;

        // Header
        if (! fgets($handle)) break;

        // Content
        while (($line = fgets($handle)) !== false) {
            $val = explode("\t", $line);
            if ($val[1] == 'resources') {
                $doctmpl .= "section\tresources\tThis section allows you to define all the resources you want\n";
                $doctmpl .= "field\ttype\tChoose the type of the resoource\n";
                $doctmpl .= "field\tdescription\tProvide a concise and accurate description of the resource\n";
                $doctmpl .= "field\tlocation\tA location can be anything:  a URL link, an absolute path in a tree, a text, ...\n";
                $section = $val[1];
                continue;
            }
            if ( $val[1] != $section ) {
                $section = $val[1];
                $doctmpl .= "section\t$section\tThis section allows you to define ...\n";
            }
            $desc='';
            if ($val[0] == 'title')
                $desc = 'Give your dataset a short name. This name will then be used to identify it and reference the metadata page via a URL. Do not use spaces or non-alphanumeric characters. Only letters, numbers and underscores are allowed.';
            $doctmpl .= "field\t".$val[0]."\t$desc\n";
            if ($val[4] =='dropbox' )
                foreach( explode(",", $val[7]) as $text)
                    $doctmpl .= "option\t".preg_replace('~[[:cntrl:]]~', '',$text)."\t\n";
        }
        fclose($handle);

    }  while(0);
    return $doctmpl;
}

// TSV to MD : Convert a terminology documentation file to Markdown
function tsv2md($filename)
{
    global $section, $section_search, $shortview, $checkboite, $txtboite, $listboite, $areaboite, $chkbxdico, $listdico;

    $md .= "# Metadata documentation\n----\n\n.\n\n";
    $section='';

    do {
        $handle = fopen($filename, "r");
        if (! $handle) { echo "File cannot be opened"; break; }

        // Header
        if (! fgets($handle)) break;

        // Content
        while (($line = fgets($handle)) !== false) {
            $val = explode("\t", $line);
            // section
            if ($val[0] == 'section') {
               if ( strlen($section)>0 ) $md .= "</div>\n\n.\n";
               $md .= "## ".strtoupper($val[1])."\n\n* ".$val[2]."\n\n<div class=\"decal\" markdown=\"1\">\n";
               $section=$val[1];
               continue;
            }
            // field
            if ($val[0] == 'field') {
               if ($section != 'resources') {
                  $md .= "### ".get_label($val[1])."\n\n* ".$val[2]."\n\n";
                  if (array_key_exists($val[1], (array)$checkboite) && $checkboite[$val[1]]['open'])
                      $md .= "* If an item does not appear in the choices proposed, it is possible to enter it manually in the field provided for this purpose. Create a line for each new item to be added.\n\n";
               } else {
                  if ($val[1]=='type') $md .= "### Type\n\n* ".$val[2]."\n\n";
                  if ($val[1]=='description') $md .= "### Description\n\n* ".$val[2]."\n\n";
                  if ($val[1]=='location') $md .= "### Location\n\n* ".$val[2]."\n\n";
               }
               continue;
            }
            // option
            if ($val[0] == 'option') {
               $md .= "    *  **".$val[1]."** :  ".$val[2]."\n";
               continue;
            }
        }
        $md .= "</div>\n\n.\n";

    }  while(0);
    return $md;
}

// TSV to HTML : Convert a terminology definition file
function tsv2html($filename)
{
    $html='';
    do {
        $handle = fopen($filename, "r");
        if (! $handle) { echo "File cannot be opened"; break; }

        $html .= <<<EOF
<style>
table { padding: 0; font-family: Tahoma; font-size: 8pt;}
</style>
EOF;

        // Header
        if (($line = fgets($handle)) === false) break;
        $html .= '<table style="border: 0px solid #B0BDCC; border-spacing: 3px;"><tr style="background-color: #4a86e8; color: white">'; // open table
        // render headers
        foreach( explode("\t", $line) as $text)
             $html .= '<th>'.$text.'</th>';

        // Content
        $cnt=0;
        while (($line = fgets($handle)) !== false) {
            $val = explode("\t", $line);
            if (($cnt%2)==0) { $TR='<tr>'; } else { $TR='<tr style="background-color:#EFF4F9;">'; }
            $html .= $TR.'<td>'.join('</td><td>',preg_replace('/,(?! )/',', $1',$val)).'</td></tr>';
            $cnt++;
        }
        $html .= '</table>'; // open table

    }  while(0);
    return $html;
}

// Check if a JSON string is valid based on the JSON-Schema
function isJSONValid($json, $schema_file="./json/pgd-mmdt-schema.json")
{
    global $JSVAPI_URL;
    $schema = file_get_contents($schema_file);
    $my_json=preg_replace('/\'/', "", $json);

    /* Creation of context options */
    $options = array(
        'http' => array(
            'method'  => 'POST',
            'header'  => "Content-Type: application/json",
            'ignore_errors' => true,
            'timeout' =>  10,
            'content' => '{"schema":'.$schema.',"json":'.$my_json.'}',
        ),
    );
    
    /* Creation of the HTTP context */
    $context  = stream_context_create($options);
    
    /* Execution of the request */
    $resp = file_get_contents($JSVAPI_URL, false, $context);

    return $resp;
}
?>
