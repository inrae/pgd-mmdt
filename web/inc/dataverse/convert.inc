<?php

//----------------------------
// Native API Dataverse
//----------------------------

function cmd_api_dataverse($cmd) {
    global $LOG;
    if ($LOG) echo "<script>console.log('".addslashes($cmd)."');</script>";
    $output_js=shell_exec($cmd);
    $output_arr = json_decode($output_js, true);
//echo "<br><pre>";  print_r($output_arr['data']); echo "</pre><br>";
    return $output_arr;
}

function get_api_dataverse($request) {
    global $API_TOKEN, $SERVER_URL;
    $cmd = 'curl -H X-Dataverse-key:'.$API_TOKEN.' -X GET --request "'.$SERVER_URL.'/api/v1/'.$request.'"';
    return cmd_api_dataverse($cmd);
}

function send_api_dataverse($request,$upload, $mode='POST') {
    global $API_TOKEN, $SERVER_URL;
    $header = 'X-Dataverse-key:'.$API_TOKEN;
    $content_type = "Content-type:application/json";
    $upload = "--upload-file ".$upload;
    $cmd='curl -H "'.$header.'" -H "'.$content_type.'" -X '.$mode.' "'.$SERVER_URL.'/api/v1/'.$request.'" '.$upload;
    return cmd_api_dataverse($cmd);
}

function send_file_dataverse($id,$file,$description,$tags,$update) {
    global $API_TOKEN, $SERVER_URL;
    $header = 'X-Dataverse-key:'.$API_TOKEN;
    $upload = '-F "file=@'.$file.'"';
    $annotation = '-F \'jsonData={"description":"'.$description.'","categories":["'.$tags.'"], "restrict":"false"}\'';
    if ($update)
       $cmd='curl -H "'.$header.'" -X POST '.$upload.' '.$annotation.' '.$SERVER_URL.'/api/v1/files/'.$id.'/replace';
    else
       $cmd='curl -H "'.$header.'" -X POST '.$upload.' '.$annotation.' '.$SERVER_URL.'/api/v1/datasets/'.$id.'/add';
    return cmd_api_dataverse($cmd);
}

function privateURL_api_dataverse($id, $get=0) {
    global $API_TOKEN, $SERVER_URL;
	$op = 'POST'; if ($get>0) $op='GET';
    $cmd = 'curl -H X-Dataverse-key:'.$API_TOKEN.' -X '.$op.' "'.$SERVER_URL.'/api/v1/datasets/'.$id.'/privateUrl"';
    return cmd_api_dataverse($cmd);
}


//----------------------------
// Convertion routines
//----------------------------

function json2dataverse($metadata, $def, $map)
{
    global $cvdir;

    // Data preparation
    $df = array();    // associative array with dataverse fields as keys
    $cvlist=array();  // the corresponding cvlist for each dataverse field if applicable
    $cv=array();
    for($i=0; $i<count($def); $i++) {
        if (! in_array( $def[$i][0],array_keys($metadata))) continue;
        if (strlen($def[$i][1])==0) continue;
        $dico=$def[$i][3];
        // If there is a dico
        if (strlen($def[$i][3])>0 && preg_match('/^cvlist:([a-zA-Z0-9_]+)/',$dico, $matches)) {
            $dico=$matches[1];
            if (! in_array($dico, $cvlist)) {
                ${$dico} = tsvToArray($cvdir.$dico.'/'.$dico.'.txt');
                array_push($cvlist, $dico);
            }
        }
        // If there is a thesaurus / ontology
        if (strlen($def[$i][3])>0 && preg_match('/^cv:([a-zA-Z0-9_]+)/',$dico, $matches)) {
            $dico=$matches[1];
            if (! in_array($dico, $cv)) {
                $idx=find_mapping($map, $dico);
                if ($idx>-1) ${$dico} = $map[$idx];
                array_push($cv, $dico);
            }
        }
        // Array of fields if there is a 'type' defined
        if (strlen($def[$i][2])>0) {
            $df[ $def[$i][2] ][] = array($def[$i][0], $def[$i][1], $def[$i][2], $dico);
        } else {
            //  Unique field
            $df[ $def[$i][1] ] = array($def[$i][0], $def[$i][2], $dico);
        }
    }
//echo "<br><pre>"; print_r($df); echo "</pre><br>"; 

 // JSON creation
    $JSONcore='{"metadataLanguage": "en", "datasetVersion": {';
    $JSONclose = '}}';
    $sep='';

    // license
    unset($df['license']); $JSONcore .= '"license": "etalab 2.0","uri": "https://spdx.org/licenses/etalab-2.0.html"';  $sep=',';
    if (isset($df['license']) && strlen($metadata[$df['license'][0]])) {
         $JSONcore .= '"license": "'.$metadata[$df['license'][0]].'"';
         $sep=',';
    }
    $JSONcore .= $sep.'"metadataBlocks": {"citation": {"displayName": "Citation Metadata", "fields": [';
    $JSONclose = ']}}'.$JSONclose;
    $sep='';

    // All other fields
    foreach( $df as $dkey => $arr ) {
      if (function_exists('getfield_'.$dkey)) {
         if(gettype($arr[0])==='array') {
             $dico=array();
             foreach($arr as $subarr) {
//print_r($dkey.', '.$subarr[0].', '.$subarr[1].', '.$subarr[2].', '.$subarr[3]); echo "<br>";
                 $dico[]=${$subarr[3]};
             }
             $json = ('getfield_'.$dkey)($metadata, $df[$dkey], $dico);
         } else {
             $dico=$arr[2];
//print_r($dkey.', '.$dico); echo "<br>";
             $json = ('getfield_'.$dkey)($metadata, $df[$dkey], ${$dico});
         }
         if (strlen($json)) $JSONcore .= $sep.$json;
         $sep=',';
      }
      if ( ($dkey==='title') && function_exists('getfield_otherId') )
         $JSONcore .= $sep.getfield_otherId($metadata);

    }
    return $JSONcore.$JSONclose;
}

function maggot2dataverse ($input_json, &$output_json)
{
	global $dataverse_conf, $mapping_conf;

	$msg='';

	do
	{
		$metadata = json_decode($input_json, true);
		if ( gettype($metadata)!== 'array' || count($metadata)==0 ) {
			$msg='Error: '.$name.' is not a valid json file !!!';
			break;
		}
	
		// Read Dataverse definition file
		if (! is_file($dataverse_conf)) {
			$msg='Error: '.$dataverse_conf.' does not exist !!!';
			break;
		}
	
		$def = tsvToArray($dataverse_conf, true);
		if (! count($def)) {
			$msg='Error: '.$dataverse_conf.' is empty !!!';
			break;
		}

		// Read Mapping definition file
		if (! is_file($mapping_conf)) {
			$msg='Error: '.$mapping_conf.' does not exist !!!';
			break;
		}

		$map = tsvToArray($mapping_conf, true);
		if (! count($map)) {
			$msg='Error: '.$mapping_conf.' is empty !!!';
			break;
		}

		// Convert JSON to Dataverse
		$output_json= json2dataverse($metadata, $def, $map);
		if (strlen($output_json)==0) {
			$msg='Error: Either no conversion has been done or the conversion has failed !!!';
			break;
		}

	} while(0);
	
	return $msg;
}

function send_resources_files($id,$resources,$path, $array_files)
{
	global $auth_senddata_file;

	$msg = '';
	foreach($resources as $arr) {
		if (strlen($arr['location'])==0 || preg_match('/^https?:/',$arr['location']) || ! file_exists($path.'/'.$arr['location']))
			continue;
		if (! file_exists($path.'/'.$auth_senddata_file)) {
			$msg .='Warning : File '.$path.'/'.$auth_senddata_file.' not found<br>';
			$msg .='Warning : No permission for file transfer<br>';
			break;
		}
		if (count($array_files)>0 && isset($array_files[$arr['location']])) {
			$op='updated';
			$output_arr = send_file_dataverse($array_files[$arr['location']],$path.'/'.$arr['location'],$arr['description'],$arr['datatype'], true);
		} else {
			$op='sent';
			$output_arr = send_file_dataverse($id,$path.'/'.$arr['location'],$arr['description'],$arr['datatype'], false);
		}
		$status = $output_arr['status'];
		if ($status==='ERROR') {
			$msg .='ERROR '.$arr['location'].': '.addslashes($output_arr['message']).'<br>';
		}
		if ($status==='OK') {
			$msg .='SUCCESS: '.$arr['location'].' '.$op.'<br>';
		}
	}
	return $msg;
}

?>
