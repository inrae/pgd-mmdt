<?php

//----------------------------
// Native API Dataverse
//----------------------------

function cmd_api_dataverse($cmd) {
	global $LOG;
	if ($LOG) echo "<script>console.log('".addslashes($cmd)."');</script>";
	$output_js=shell_exec($cmd);
	if ($LOG) echo "<script>console.log('".$output_js."');</script>";
	$output_arr = json_decode($output_js, true);
// echo "<br><pre>";  print_r($output_arr['data']); echo "</pre><br>";
	return $output_arr;
}

function get_api_dataverse($request) {
	global $DV_CURL, $DV_API_URL;
	$cmd = $DV_CURL.' -X GET "'.$DV_API_URL.'/'.$request.'"';
	return cmd_api_dataverse($cmd);
}

function send_api_dataverse($request, $upload, $mode='POST') {
	global $DV_CURL, $DV_API_URL;
	$content_type = "Content-type:application/json";
	$upload = "--upload-file ".$upload;
	$cmd=$DV_CURL.' -H "'.$content_type.'" -X '.$mode.' "'.$DV_API_URL.'/'.$request.'" '.$upload;
	return cmd_api_dataverse($cmd);
}

function send_file_dataverse($id, $file, $description, $tags, $update) {
	global $DV_CURL, $DV_API_URL;
	$upload = '-F "file=@'.$file.'"';
	$annotation = '-F \'jsonData={"description":"'.$description.'","categories":["'.$tags.'"], "restrict":"false"}\'';
	if ($update)
		$cmd=$DV_CURL.' -X POST '.$upload.' '.$annotation.' '.$DV_API_URL.'/files/'.$id.'/replace';
	else
		$cmd=$DV_CURL.' -X POST '.$upload.' '.$annotation.' '.$DV_API_URL.'/datasets/'.$id.'/add';
	return cmd_api_dataverse($cmd);
}

function privateURL_api_dataverse($id, $get=0) {
	global $DV_CURL, $DV_API_URL;
	$op = 'POST'; if ($get>0) $op='GET';
	$cmd = $DV_CURL.' -X '.$op.' "'.$DV_API_URL.'/datasets/'.$id.'/privateUrl"';
	return cmd_api_dataverse($cmd);
}

//----------------------------
// Convertion routines
//----------------------------

function isEmptyBlock ($hash, $block) {
	return substr($hash[$block], -1) == "[" ? True : False;
}

function json2dataverse($metadata, $def, $map)
{
	global $cvdir, $biomedicalCV, $DV_LICENSE_ETALAB;

	// Data preparation
	$df = array();    // associative array with dataverse fields as keys
	$cvlist=array();  // the corresponding cvlist for each dataverse field if applicable
	$cv=array();
	for($i=0; $i<count($def); $i++) {
		if (! in_array( $def[$i][0],array_keys($metadata))) continue;
		if (strlen($def[$i][1])==0) continue;
		$dico=$def[$i][3];
		$block = "citation";
		if (count($def[$i])>3 && strlen($def[$i][4])>0) $block = $def[$i][4];
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
			$df[ $def[$i][2] ][] = array($def[$i][0], $def[$i][1], $def[$i][2], $dico, $block);
		} else {
			//  Unique field
			$df[ $def[$i][1] ] = array($def[$i][0], $def[$i][2], $dico, $block);
		}
	}
//echo "<br><pre>"; print_r($df); echo "</pre><br>"; 

	// JSON creation
	$JSONfull='{"metadataLanguage": "en", "datasetVersion": {';
	$JSONclose = '}}';
	$sep='';

	// license
	if ($DV_LICENSE_ETALAB) unset($df['license']); // France
	if (isset($df['license']) && strlen($metadata[$df['license'][0]])) {
		$JSONfull .= '"license": "'.$metadata[$df['license'][0]].'"';
		$sep=',';
	} else {
		$JSONfull .= '"license": "etalab 2.0","uri": "https://spdx.org/licenses/etalab-2.0.html"';
		$sep=',';
	}

	// metadataBlocks
	$JSONfull .= $sep.'"metadataBlocks": {';
	$JSONclose = '}'.$JSONclose;

	// Blocks
	$JSONBlocks = array(
		"citation" => '"citation": {"displayName":"Citation Metadata", "name":"citation", "fields": [',
		"biomedical" => '"biomedical": {"displayName":"Life Sciences Metadata", "name":"biomedical", "fields": ['
	);
	$JSONcloseBlock = ']}';

	// Retrieve all CVs from the "biomedical" block
	$biomedicalCV = get_biomedicalCV();

	// All fields in Blocks
	foreach( $df as $dkey => $arr ) {
		if (function_exists('getfield_'.$dkey)) {
			$block = '';
			if(gettype($arr[0])==='array') {
				$dico=array();
				foreach($arr as $subarr) {
					if (strlen($block)==0) $block = $subarr[4];
//print_r($dkey.', '.$subarr[0].', '.$subarr[1].', '.$subarr[2].', '.$subarr[3].', '.$block); echo "<br>";
					if (strlen($subarr[3]) && isset( ${$subarr[3]} ))
						$dico[]=${$subarr[3]};
				}
				$json = ('getfield_'.$dkey)($metadata, $df[$dkey], $dico);
			} else {
				$dico=$arr[2];
				$block = $arr[3];
//print_r($dkey.', '.$dico.', '.$block); echo "<br>";
				$dic = strlen($dico) && isset( ${$dico} ) ? ${$dico} : "";
				$json = ('getfield_'.$dkey)($metadata, $df[$dkey], $dic);
			}
			$sep = isEmptyBlock($JSONBlocks,$block) ? '' : ',';
			if (strlen($json)) { $JSONBlocks[$block] .= $sep.$json; }
		}
		if ( ($dkey==='title') && function_exists('getfield_otherId') ) {
			$block = 'citation';
			$sep = isEmptyBlock($JSONBlocks,$block) ? '' : ',';
			$JSONBlocks[$block] .= $sep.getfield_otherId($metadata);
		}
	}

	// Final JSON 
	$JSONfull .= $JSONBlocks['citation'].$JSONcloseBlock;
	$JSONfull .= isEmptyBlock($JSONBlocks,$block) ? '' : ','.$JSONBlocks['biomedical'].$JSONcloseBlock;
	$JSONfull .= $JSONclose;
	return $JSONfull;
}

function maggot2dataverse ($input_json, &$output_json)
{
	global $dataverse_conf, $mapping_conf;

	$msg='';

	do
	{
		$metadata = json_decode($input_json, true);
		if ( gettype($metadata)!== 'array' || count($metadata)==0 ) {
			$msg='{"Error":"'.$name.' is not a valid json file !!!"}';
			break;
		}

		// Read Dataverse definition file
		if (! is_file($dataverse_conf)) {
			$msg='{"Error":"'.$dataverse_conf.' does not exist !!!"}';
			break;
		}

		$def = tsvToArray($dataverse_conf, true);
		if (! count($def)) {
			$msg='{"Error":"'.$dataverse_conf.' is empty !!!"}';
			break;
		}

		// Read Mapping definition file
		if (! is_file($mapping_conf)) {
			$msg='{"Error":"'.$mapping_conf.' does not exist !!!"}';
			break;
		}

		$map = tsvToArray($mapping_conf, true);
		if (! count($map)) {
			$msg='{"Error":"'.$mapping_conf.' is empty !!!"}';
			break;
		}

		// check if all "cv" are defined in the mapping
		$map_names = [];
		foreach( $map as $arr ) array_push($map_names, $arr[0]);
		$arr_msg = [];
		foreach( $def as $arr1 ) {
			if (!preg_match('/cv:/', $arr1[3])) continue;
			$cvname = explode(':', $arr1[3])[1];
			if (!in_array($cvname, $map_names))
				array_push($arr_msg, '{"Error":"Field '.$arr1[0].', '.$cvname.' does not defined in the mapping file"}');
		}
		if (count($arr_msg)) {
			$msg = '['.implode(',', $arr_msg).']';
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

function send_resources_files($id, $resources, $path, $array_files)
{
	global $auth_senddata_file;

	$msg = '';
	foreach($resources as $arr) {
		if (strlen($arr['location'])==0 || 
			preg_match('/^https?:/',$arr['location']) || 
			! file_exists($path.'/'.$arr['location']))
			continue;
		if (! file_exists($path.'/'.$auth_senddata_file)) {
			$msg .='Warning : File '.$path.'/'.$auth_senddata_file.' not found<br>';
			$msg .='Warning : No permission for file transfer<br>';
			break;
		}
		if (count($array_files)>0 && isset($array_files[$arr['location']])) {
			$op='updated';
			$output_arr = send_file_dataverse($array_files[$arr['location']], $path.'/'.$arr['location'], $arr['description'], $arr['datatype'], true);
		} else {
			$op='sent';
			$output_arr = send_file_dataverse($id, $path.'/'.$arr['location'], $arr['description'], $arr['datatype'], false);
		}
		$status = $output_arr['status'];
		if ($status==='ERROR') {
			$msg .= '<b>'.$arr['location'].'</b>: '.addslashes($output_arr['message']).'<br>';
		}
		if ($status==='OK') {
			$msg .= '<b>'.$arr['location'].'</b> '.$op.' with success<br>';
		}
	}
	return $msg;
}

?>
