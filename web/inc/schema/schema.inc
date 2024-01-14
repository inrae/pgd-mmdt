<?php
#
# JSON-LD implementation 
# Preliminary work (sept 2023)
#
# Auteur(s): D.JACOB 
# Copyright: (c) INRAE - 2022-2023
#

$ror_cache = array();

function get_license_url ($licenseId)
{
	global $url_raw_licences;
	try {
		$json_licenses = file_get_contents($url_raw_licences);
		$licenses = json_decode($json_licenses,true);
		$reference = $licenseId;
		foreach($licenses['licenses'] as $arr) {
			$pattern=strtolower($licenseId);
			if (preg_match('/etalab*/', $pattern))
				$pattern = 'etalab-2.0';
			if (preg_match('/'.$pattern.'*/', strtolower($arr['licenseId']))) {
				$reference = $arr['reference'];
				break;
			}
		}
	} catch (Exception $e) {
		$reference = $licenseId;
	}
	return $reference;
}

// See https://ror.readme.io/docs/rest-api
function get_ROR_url ($organization)
{
	global $ror_cache, $url_ror;
	try {
		$url_ror_id = '';
		if (array_key_exists($organization, $ror_cache)) {
			$url_ror_id = $ror_cache[$organization];
		} else {
			$json_ror = file_get_contents($url_ror.urlencode($organization));
			$ror = json_decode($json_ror,true);
			if (count($ror)>0) {
				$url_ror_id = $ror['items'][0]['id'];
				$ror_cache[$organization] = $url_ror_id;
			}
		}
	} catch (Exception $e) {
		$url_ror_id = '';
    }
	return $url_ror_id;
}

function addComma (&$str) {	if (substr($str,-1) !== ',') $str = $str.','; }
function removeComma (&$str) {	$len = strlen($str); if (substr($str,-1) == ',') $str = substr($str,0,$len-1); }

//----------------------------------------
// Generate the schema.org JSON-LD file (output_json)
// based on the Maggot JSON file (input_json)
//----------------------------------------

function json2schema($metadata, $def, $map)
{
	global $cvdir, $id_metadata;

	// Index of columns
	$CS=0; $CF=1; $CT=2; $CL=3; $CA=4; $CM=5; $CP=6; $VT=7; $VS=8; $CI=9; $PR=10;
	$CN=1; $CU=2;

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
	while (strlen($def[$cnt][0])==0) {
		$arr=$def[$cnt];
		if (! in_array( $arr[$CF],array_keys($metadata))) { $cnt++; continue; }
		// If there is a dico
		if (strlen($arr[$VS])>0 && preg_match('/^cvlist:([a-zA-Z0-9_]+)/',$arr[$VS], $matches)) {
			$dico=$matches[1];
			if (! in_array($dico, $cvlist)) {
				${$dico} = tsvToArray($cvdir.$dico.'/'.$dico.'.txt');
				array_push($cvlist, $dico);
			}
		}
		// If there is a thesaurus / ontology
		if (strlen($arr[$VS])>0 && preg_match('/^cv:([a-zA-Z0-9_]+)/',$arr[$VS], $matches)) {
			$dico=$matches[1];
			if (! in_array($dico, $cv)) {
				$idx=find_mapping($map, $dico);
				if ($idx>-1) ${$dico} = $map[$idx];
				array_push($cv, $dico);
			}
		}
		$cnt++;
	}

#echo "<br><pre>"; print_r ($metadata); echo "</pre><br>";
#$dico ='people'; echo "<br><pre>"; print_r (${$dico}); echo "</pre><br>"; $msg='break';

	//----------------------------------------
	// Parse the schema definition file
	//----------------------------------------
	$level = 0;
	$level_type = '';
	$arrname = $fieldname = $p_arrname = $p_fieldname = '';
	$jsoncore = '';

	$cnt=1;
	while (strlen($def[$cnt][$CS])==0) {
		$arr=$def[$cnt];    // current line
		$arr2=$def[$cnt+1]; // following line
		$level = $arr[$CL];
		do {
			if ((strlen($arr[$CF])==0 && strlen($arr[$CT])==0)|| $def[$cnt][$CS] == '#context') break;

	// Open an array at level 0
			if ($arr[$CA]==$Y && $arr[$CM]==$Y) {
				if (array_key_exists($arr[$CF], $metadata) && count($metadata[$arr[$CF]])>0) {
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
				if (array_key_exists($arr[$CF], $metadata)) {
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
				if (array_key_exists($arr[$CF], $metadata)) {
					if ($level == 0 && $arr[$VT]=='#VALUE') {
						$fieldname=$arr[$CF]; $arrname = $arr[$CT];
						$jsoncore .= '"'.$arrname.'":["'.join('", "', $metadata[$fieldname]).'"]';
						if (strlen($arr2[$CA])>0 && strlen($arr2[$CM])>0) addComma($jsoncore);
						break;
					}
				}
				break;
			}

	// Level 0
			if ($level==0 && strlen($arr[$CF])==0 && ! in_array($arr[$VT], $ValueTypes) ) {
				$jsoncore .= '"'.$arr[$CT].'":"'.$arr[$VT].'"';
				if (strlen($arr2[$CA])>0 && strlen($arr2[$CM])>0) addComma($jsoncore);
				break;
			}
			if ($level==0 && $arr[$CF] == '@type') {
				$jsoncore .= '"@type":"'.$arr[$VT].'"';
				if (strlen($arr2[$CA])>0 && strlen($arr2[$CM])>0) addComma($jsoncore);
				break;
			}
			if ($level==0 && $arr[$CF] == '@id') {
				$jsoncore .= '"@id":"'.$arr[$PR].$id_metadata.'"';
				if (strlen($arr2[$CA])>0 && strlen($arr2[$CM])>0) addComma($jsoncore);
				break;
			}
			if ($level==0 && $arr[$CA]==$N && $arr[$CM]==$N && array_key_exists($arr[$CF], $metadata)) {
				$value = $metadata[$arr[$CF]];
				if ($arr[$CT] == 'schema:license') $value = get_license_url($value);
				$jsoncore .= '"'.$arr[$CT].'":"'.$arr[$PR].$value.'"';
				if (strlen($arr2[$CA])>0 && strlen($arr2[$CM])>0) addComma($jsoncore);
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
					if (strlen($def[$cnt+$J][$CA])==0 || strlen($def[$cnt+$J][$CM])==0) break;
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
					for($k=0; $k<$nbitems; $k++) {
						$res2=array();
						$url_ok = 1;
						for($i=0; $i<$J; $i++) {
							$key = $defobj[$i][$CP]; $value = $defobj[$i][$VT];
							if ($value == '#VALUE' && array_key_exists($defobj[$i][$VS],$meta[$k]))
								$value = $defobj[$i][$PR].$meta[$k][$defobj[$i][$VS]];
							if ($defobj[$i][$VS]=='location') {
								if ($arrname == 'schema:hasPart' && ! preg_match('/^http*/', $value)) { $url_ok=0; break; }
								if ($arrname !== 'schema:hasPart' && preg_match('/^http*/', $value)) { $url_ok=0; break; }
							}
							if ($value !== '#VALUE')
								array_push($res2, '"'.$key.'":"'.$value.'"');
						}
						if ($url_ok)
							array_push($res, '{'.join(',',$res2).'}');
					}
					if (count($res)>0)
						$jsoncore .= join(',', $res);
				}
				$objtype = '';
			// Not Resources
				if ($fieldname !== 'resources') for($k=0; $k<$nbitems; $k++) {
					$jsoncore .= '{';
					for($i=0; $i<$J; $i++) {
					// Get the level value
						$p_level = $level; $level = $defobj[$i][$CL];
					// if an object at level 2
						if ($defobj[$i][$CM]==$Y) {
							$jsoncore .= '"'.$defobj[$i][$CP].'":{';
							continue;
						}
					// Get the key/property value
						$key = $defobj[$i][$CP];
						if ($key == '@type')
							$objtype = $defobj[$i][$VT];
					// if relevant, close object at level 2
						if ($level<$p_level) {
							$jsoncore .= '}';
							if ($i<$J && $defobj[$i][$CA]==$N && $defobj[$i][$CM]==$N)
								addComma($jsoncore);
							$level=$p_level;
						}
					// Get the value
						$value = $defobj[$i][$VT];
					// Value depending on its statement (#VALUE, #DICO, #ONTO)
						if ($value == '#VALUE') {
							$value = $meta[$k];
							if ($objtype=='schema:Person') $value = str_replace(' ',', ',$value);
						}
						if ($value == '#DICO') {
							if (strlen($defobj[$i][$VS])>0 && preg_match('/^cvlist:([a-zA-Z0-9_]+)/',$defobj[$i][$VS], $matches)) {
								$dico=$matches[1];
								$opt = ($dico == 'people') ? array(0,' ',1) : array(0);
								$idx=find_dico(${$dico}, $meta[$k], $opt);
								if ($idx>-1)
									$value = ${$dico}[$idx][$defobj[$i][$CI]];
								if ($objtype=='schema:Organization' && $key=='name') {
									$url_ror_id=get_ROR_url($value);
									if ($url_ror_id !== $value)
										$jsoncore .= '"identifier":"'.$url_ror_id.'",';
								}
							}
						}
						if ($value == '#ONTO') {
							if (strlen($defobj[$i][$VS])>0 && preg_match('/^cv:([a-zA-Z0-9_]+)/',$defobj[$i][$VS], $matches)) {
								$dico=$matches[1];
								$out = get_term_info(${$dico}, $meta[$k]);
								if (! empty($out))
									$value = $out[$defobj[$i][$CI]];
							}
						}
					// if value is not empty
						if (strlen(strval($value))>0) {
							$value = $defobj[$i][$PR].$value;
							if (! in_array($value, $ValueTypes)) {
								$jsoncore .= '"'.$key.'":"'.$value.'"';
								if ($i<($J-1))
									addComma($jsoncore);
							} elseif ($i==($J-1))
								$jsoncore = rtrim($jsoncore, ","); 
						}
					}
					if ($level == 2) { // close object at level 2
						removeComma($jsoncore); 
						$jsoncore .= '}';
						$level=1;
					}
					$jsoncore .= '}'; // close object at level 1
					if ($k<(count($meta)-1))
						addComma($jsoncore);
				}
				$cnt = $cnt + $J-1;
				$arr2=$def[$cnt+1];
			}

	// Close an object or an array at level 1
			if ($level == 1 && ($arr2[$CA]==$Y || $arr2[$CM]==$Y || strlen($arr2[$CF])==0 || $arr2[$CS] == '#context')) {
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
				if ($arr2[$CS] !== '#context' && strlen($arr2[$CF])>0)
					addComma($jsoncore);
				$level = 0; $level_type = '';
			}

		} while (0);
		$cnt++;
	}

	$output_json = '';
	if ($def[$cnt][$CS] == '#context') {
		$cnt++;
		$jsoncontext =array();
		while (strlen($def[$cnt][$CN])>0) {
			$jsoncontext[] = '"'.$def[$cnt][$CN].'":"'.$def[$cnt][$CU].'"';
			$cnt++;
		}
		removeComma($jsoncore);
		$jsoncore .= ',"sdDatePublished":"'.date("Y-m-d H:i:s").'"';
		$output_json = '{ "@context": {'.join(', ',$jsoncontext).'}, '.$jsoncore.'}';
	}

	return $output_json;
}

function maggot2schema ($input_json, &$output_json)
{
	global $jsonld_conf, $mapping_conf;
	$msg='';

	do
	{
		// Get Metadata
		$metadata = json_decode($input_json, true);
		if ( gettype($metadata)!== 'array' || count($metadata)==0 ) {
			$msg='Error: '.$name.' is not a valid json file !!!';
			break;
		}

		// Read schema definition file
		if (! is_file($jsonld_conf)) {
			$msg='Error: '.$jsonld_conf.' does not exist !!!';
			break;
		}
		$def = tsvToArray($jsonld_conf, false);
		if (! count($def)) {
			$msg='Error: '.$jsonld_conf.' is empty !!!';
			break;
		}

		if ($def[0][0] !== '#datasetField') {
			$msg='Error: '.$jsonld_conf.' seems have a wrong format!!!';
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

		$output_json = json2schema($metadata, $def, $map);

	} while(0);

	return $msg;
}

?>
