<?php

//----------------------------
// Native API Dataverse
//----------------------------

function cmd_api_zenodo($cmd) {
	global $LOG;
	if ($LOG) echo "<script>console.log('".addslashes($cmd)."');</script>";
	$output_js=shell_exec($cmd);
	$output_arr = json_decode($output_js, true);
	return $output_arr;
}

function get_api_zenodo($id='') {
	global $ZENODO_API_TOKEN, $ZENODO_SERVER_URL;
	if (strsize($id) > 0)
		$cmd = 'curl --request GET '.$ZENODO_SERVER_URL.'/api/deposit/depositions/'.$id.'?access_token='.$ZENODO_API_TOKEN;
	else
		$cmd = 'curl --request GET '.$ZENODO_SERVER_URL.'/api/deposit/depositions?access_token='.$ZENODO_API_TOKEN;
	return cmd_api_zenodo($cmd);
}

function send_api_zenodo($content, $mode='POST') {
	global $ZENODO_API_TOKEN, $ZENODO_SERVER_URL;
	$content_type = "Content-type:application/json";
	$cmd='curl -H "'.$content_type.'" -X '.$mode.' --data \''.$content.'\' '.$ZENODO_SERVER_URL.'/api/deposit/depositions?access_token='.$ZENODO_API_TOKEN;
	return cmd_api_zenodo($cmd);
}

function send_file_zenodo($bucket_link, $file) {
	global $ZENODO_API_TOKEN;
	$cmd='curl --upload-file '.$file.' --request PUT '.$bucket_link.'/'.basename($file).'?access_token='.$ZENODO_API_TOKEN;
	return cmd_api_zenodo($cmd);
}

//----------------------------
// Convertion routines
//----------------------------

function addComma (&$str) {	if (substr($str,-1) !== ',') $str = $str.','; }
function removeComma (&$str) {	$len = strlen($str); if (substr($str,-1) == ',') $str = substr($str,0,$len-1); }

function json2zenodo($metadata, $def, $map, $COMMUNITIES='')
{
	global $ZENODO, $cvdir;

	// Index of columns
	$CF=0; $CT=1; $CL=2; $CA=3; $CM=4; $CP=5; $VT=6; $VS=7; $CI=8; $PR=9;

	// Boolean values
	$Y='TRUE'; $N='FALSE';

	// Types of values
	$ValueTypes = array('#VALUE', '#DICO', '#ONTO');

	//----------------------------------------
	// Preparation / Initialization
	//----------------------------------------

	// Read dictionnaries and ontlogy definitions
	$cvlist=array();  // the corresponding cvlist for each dataverse field if applicable
	$cv=array();
	$cnt=1;
	while (isset($def[$cnt]) && strsize($def[$cnt][1])>0) {
		$arr=$def[$cnt];
		if (! in_array( $arr[$CF],array_keys($metadata))) { $cnt++; continue; }
		// If there is a dico
		if (strsize($arr[$VS])>0 && preg_match('/^cvlist:([a-zA-Z0-9_]+)/',$arr[$VS], $matches)) {
			$dico=$matches[1];
			if (! in_array($dico, $cvlist)) {
				${$dico} = tsvToArray($cvdir.$dico.'/'.$dico.'.txt');
				array_push($cvlist, $dico);
			}
		}
		// If there is a thesaurus / ontology
		if (strsize($arr[$VS])>0 && preg_match('/^cv:([a-zA-Z0-9_]+)/',$arr[$VS], $matches)) {
			$dico=$matches[1];
			if (! in_array($dico, $cv)) {
				$idx=find_mapping($map, $dico);
				if ($idx>-1) ${$dico} = $map[$idx];
				array_push($cv, $dico);
			}
		}
		$cnt++;
	}

	//----------------------------------------
	// Parse the Zenodo definition file
	//----------------------------------------
	$level = 0;
	$level_type = '';
	$arrname = $fieldname = $p_arrname = $p_fieldname = '';
	$jsoncore = '';

	if (strsize($COMMUNITIES)) {
		$cnt=0;
		$def[$cnt] = [ '', 'communities', 0, $N, $N, '', $COMMUNITIES, '', '', '' ];
	} else {
		$cnt=1;
	}
	while (isset($def[$cnt]) && isset($def[$cnt][$CT]) && strsize($def[$cnt][$CT])>0) {
		$arr=$def[$cnt];    // current line
		$arr2=$def[$cnt+1]; // following line
		$level = $arr[$CL];
		do {
			if (strsize($arr[$CT])==0) break;
	// Open an array at level 0
			if ($arr[$CA]==$Y && $arr[$CM]==$Y) {
				if (strsize($arr[$CF])>0 && array_key_exists($arr[$CF], $metadata) && count($metadata[$arr[$CF]])>0) {
					$cond1 = ($level == 0);
					$cond2 = ($level == 1 && $arrname !== $arr[$CT] && $fieldname !== $arr[$CF]);
					if ($cond1 || $cond2) {
						$jsoncore .= '"'.$arr[$CT].'":[';
					}
					$level = 1; $level_type = 'array';
					$p_fieldname = $fieldname; $p_arrname = $arrname;
					$fieldname=$arr[$CF]; $arrname = $arr[$CT];
				}
				break;
			}

	// Open a simple object at level 0 or at level 1
			if ($arr[$CA]==$N && $arr[$CM]==$Y) {
				if (strsize($arr[$CF])>0 && array_key_exists($arr[$CF], $metadata)) {
					$cond1 = ($level == 0 && $arrname !== $arr[$CT] && $fieldname !== $arr[$CF]);
					if ($cond1) {
						$fieldname=$arr[$CF]; $arrname = $arr[$CT];
						if ($level == 0) {
							$jsoncore .= '"'.$arrname.'":{';
							$level = 1; $level_type = 'object';
						}
					}
				}
				break;
			}

	// Simple array at level 0
			if ($arr[$CA]==$Y && $arr[$CM]==$N) {
				if (strsize($arr[$CF])>0 && array_key_exists($arr[$CF], $metadata)) {
					if ($level == 0 && $arr[$VT]=='#VALUE') {
						$fieldname=$arr[$CF]; $arrname = $arr[$CT];
						$jsoncore .= '"'.$arrname.'":["'.join('", "', $metadata[$fieldname]).'"]';
						if (strsize($arr2[$CA])>0 && strsize($arr2[$CM])>0) addComma($jsoncore);
						break;
					}
				}
				break;
			}

	// Level 0
			if ($level==0 && strsize($arr[$CF])==0 && ! in_array($arr[$VT], $ValueTypes) ) {
				if ($arr[$CT]=='communities') {
					$values = explode(',', $arr[$VT]);
					$jsonval = array();
					foreach ($values as $value) $jsonval[] .= '{"identifier":"'.$value.'"}';
					$jsoncore .= '"communities": [ '.implode(',',$jsonval).']';
				} elseif (in_array($arr[$VT], array('true','false')))
					$jsoncore .= '"'.$arr[$CT].'":'.$arr[$VT];
				else
					$jsoncore .= '"'.$arr[$CT].'":"'.$arr[$VT].'"';
				if (strsize($arr2[$CA])>0 && strsize($arr2[$CM])>0) addComma($jsoncore);
				break;
			}
			if ($level==0 && $arr[$CA]==$N && $arr[$CM]==$N && array_key_exists($arr[$CF], $metadata)) {
				$value = str_replace('"', '\"',  str_replace("'", "`", $metadata[$arr[$CF]]));
				if ($arr[$CT]=='access_right')
					$value=$ZENODO['ACCESS_RIGHTS'][$value];
				$jsoncore .= '"'.$arr[$CT].'":"'.$arr[$PR].$value.'"';
				if (strsize($arr2[$CA])>0 && strsize($arr2[$CM])>0) addComma($jsoncore);
				break;
			}

	// Simple objet at level 1 -
			if ($level == 1 && $level_type == 'object') {
				$value = $arr[$VT];
				if ($value == '#VALUE') $value = $metadata[$arr[$CF]];
				$jsoncore .= '"'.$arr[$CP].'":"'.$arr[$PR].$value.'"';
				if ($arr2[$CA]==$N && $arr2[$CM]==$N) addComma($jsoncore);
			}

	// Array of objects at level 1
			if ($level == 1 && $level_type == 'array' && $arr[$CA]==$N && $arr[$CT]==$arrname) {
				// Get all lines corresponding to the object definition
				$defobj = array();
				$J=0;
				while ($def[$cnt+$J][$CA]==$N && $def[$cnt+$J][$CL]>0) {
					$defobj[$J] = $def[$cnt+$J];
					$J++;
					if (!isset($def[$cnt+$J][$CT]) || strsize($def[$cnt+$J][$CT])==0) break;
				}
				// Get the current field and check if there are metadata corresponding to it
				if ($defobj[0][$CF] !== $fieldname) $fieldname = $defobj[0][$CF];
				if (array_key_exists($fieldname, $metadata)) {
					$meta = $metadata[$fieldname];
					$nbitems = count($meta);
				} else {
					$nbitems = 0;
				}
				// Not the first object within the same array ?
				if ($arrname == $p_arrname && $fieldname !== $p_fieldname && $nbitems>0)
					addComma($jsoncore);
			// Resources
				if ($fieldname == 'resources') {
					$res = array();
					//array_push($res,  '{ "identifier": "maggot:'.$metadata['title'].'", "relation": "isAlternateIdentifier","scheme": "other" }' );
					for($k=0; $k<$nbitems; $k++) {
						$res2=array();
						$url_ok = 1; $scheme='url';
						for($i=0; $i<$J; $i++) {
							$key = $defobj[$i][$CP]; $value = $defobj[$i][$VT];
							if ($value == '#VALUE' && array_key_exists($defobj[$i][$VS],$meta[$k]))
								$value = $defobj[$i][$PR].$meta[$k][$defobj[$i][$VS]];
							if ($defobj[$i][$VS]=='location') {
								if (! preg_match('/^http*/', $value)) { $url_ok=0; break; }
								if (preg_match('/^https?:..doi.org.(.+)/', $value, $matches)) {
									$value = $matches[1]; $scheme='doi';
								}
							}
							if ($defobj[$i][$VS]=='datatype' && array_key_exists($value,$ZENODO['RELATION_TYPES'])) {
								$value = $ZENODO['RELATION_TYPES'][$value];
							}
							if ($value !== '#VALUE')
								array_push($res2, '"'.$key.'":"'.$value.'"');
						}
						if ($url_ok)
							array_push($res, '{'.join(',',$res2).', "scheme":"'.$scheme.'"}');
					}
					if (count($res)>0)
						$jsoncore .= join(',', $res);
				}
			// Not Resources
				if ($fieldname !== 'resources') for($k=0; $k<$nbitems; $k++) {
					$jnode = ''; $nbfields=0; $cntval=0;
					for($i=0; $i<$J; $i++) {
					// Type of the object
						$objtype = $defobj[$i][$CT];
					// Get the level value
						$p_level = $level; $level = $defobj[$i][$CL];
					// if an object at level 2
						if ($defobj[$i][$CM]==$Y) {
							$jnode .= '"'.$defobj[$i][$CP].'":{';
							continue;
						}
					// Get the key/property value
						$key = $defobj[$i][$CP]; $nbfields++;
						$fpeople = in_array($objtype, array('creators','contributors')) ? 1 : 0 ; 
					// if relevant, close object at level 2
						if ($level<$p_level) {
							$jnode .= '}';
							if ($i<$J && $defobj[$i][$CA]==$N && $defobj[$i][$CM]==$N)
								addComma($jnode);
							$level=$p_level;
						}
					// Get the value
						$value = $defobj[$i][$VT];
					// Value depending on its statement (#VALUE, #DICO, #ONTO)
						if ($value == '#VALUE') {
							$value = $meta[$k];
							if ($fpeople) $value = str_replace(' ',', ',$value);
						}
						if ($value == '#DICO') {
							if (strsize($defobj[$i][$VS])>0 && preg_match('/^cvlist:([a-zA-Z0-9_]+)/',$defobj[$i][$VS], $matches)) {
								$dico=$matches[1];
								$opt = ($dico == 'people') ? array(0,' ',1) : array(0);
								$idx=find_dico(${$dico}, $meta[$k], $opt);
								if ($idx>-1)
									$value = ${$dico}[$idx][$defobj[$i][$CI]];
							}
						}
						if ($value == '#ONTO') {
							if (strsize($defobj[$i][$VS])>0 && preg_match('/^cv:([a-zA-Z0-9_]+)/',$defobj[$i][$VS], $matches)) {
								$dico=$matches[1];
								$out = get_term_info(${$dico}, $meta[$k]);
								if (! empty($out))
									$value = $out[$defobj[$i][$CI]];
							}
						}
					// if Type == 'grants'
						if (strsize(strval($value))>0 && $objtype=='grants') {
							if (preg_match('/^ANR-[0-9_]+-.+/',$value)) $value = $ZENODO['GRANTS_DOI']['ANR'].'::'.$value;
							elseif (preg_match('/^([0-9_]+)/',$value,$matches)) $value = $ZENODO['GRANTS_DOI']['EU'].'::'.$matches[1];
						}
					// if value is not empty
						if (strsize(strval($value))>0) {
							$value = $defobj[$i][$PR].$value;
							if (! in_array($value, $ValueTypes)) {
								$cntval++;
								$jnode .= '"'.$key.'":"'.$value.'"';
								if ($i<($J-1))
									addComma($jnode);
							} elseif ($i==($J-1))
								$jnode = rtrim($jnode, ","); 
						}
					}
					if ($level == 2) { // close object at level 2
						removeComma($jnode); 
						$jnode .= '}';
						$level=1;
					}
					if (strsize($jnode) && ($objtype!=='subjects' || $cntval==$nbfields)) {
						$jsoncore .= '{'.$jnode.'}'; // object at level 1
						if ($k<(count($meta)-1))
							addComma($jsoncore);
					}
				}
				$cnt = $cnt + $J-1;
				$arr2=isset($def[$cnt+1]) ? $def[$cnt+1] : [];
			}

	// Close an object or an array at level 1
			if (empty($arr2) || $level == 1 && ($arr2[$CA]==$Y || $arr2[$CM]==$Y || strsize($arr2[$CF])==0)) {
				if ($fieldname !== 'resources' && ($arrname == $arr2[$CT] || $fieldname == $arr2[$CF]))
					break;
				if ($level_type=='object') {
					removeComma($jsoncore); 
					$jsoncore .= '}';
				}
				if ($level_type=='array') {
					removeComma($jsoncore); 
					$jsoncore .= ']';
				}
				if (isset($arr2[$CT]) && strsize($arr2[$CT])>0)
					addComma($jsoncore);
				$level = 0; $level_type = '';
			}

		} while (0);
		$cnt++;
	}

	removeComma($jsoncore);
	$jsoncore .= ',"publication_date":"'.date("Y-m-d").'"';
	$output_json = '{"metadata": {'.$jsoncore.'}}';

	return $output_json;
}


function maggot2zenodo ($input_json, &$output_json)
{
	global $zenodo_conf, $mapping_conf, $COMMUNITIES;
	$msg='';

	do
	{
		// Get Metadata
		$metadata = json_decode($input_json, true);
		if ( gettype($metadata)!== 'array' || count($metadata)==0 ) {
			$msg='{"Error":"'.$name.' is not a valid json file !!!"}';
			break;
		}

		// Read schema definition file
		if (! is_file($zenodo_conf)) {
			$msg='{"Error":"'.$zenodo_conf.' does not exist !!!"}';
			break;
		}
		$def = tsvToArray($zenodo_conf, false);
		if (! count($def)) {
			$msg='{"Error":"'.$zenodo_conf.' is empty !!!"}';
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
			if (!preg_match('/cv:/', $arr1[7])) continue;
			$cvname = explode(':', $arr1[7])[1];
			if (!in_array($cvname, $map_names))
				array_push($arr_msg, '{"Error":"Field '.$arr1[0].', '.$cvname.' does not defined in the mapping file"}');
		}
		if (count($arr_msg)) {
			$msg = '['.implode(',', $arr_msg).']';
			break;
		}

		$output_json = json2zenodo($metadata, $def, $map, $COMMUNITIES);

	} while(0);

	return $msg;
}

?>
