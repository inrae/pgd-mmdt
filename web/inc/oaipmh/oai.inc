<?php
#
# OAI-PMH very quick implementation 
# Preliminary work (sept 2023)
#
## OAI-PMH Specifications version 2
## http://www.openarchives.org/OAI/openarchivesprotocol.html
#
## OAI-PMH Data Provider Validation and Registration
## http://www.openarchives.org/Register/ValidateSite
#
# Auteur(s): D.JACOB 
# Copyright: (c) INRAE - 2022-2023
#

//----------------------------------------
// Get metadata 
//----------------------------------------

function getMetadata ($id_metadata='')
{
	global $txtboite, $listboite, $checkboite, $multiboite, $areaboite;
	
	// Options
	$options = array('projection'=>[]);
	foreach(array_keys((array)$txtboite) as $key) $options['projection'][$key]=true;
	foreach(array_keys((array)$listboite) as $key) $options['projection'][$key]=true;
	foreach(array_keys((array)$checkboite) as $key) $options['projection'][$key]=true;
	foreach(array_keys((array)$multiboite) as $key) $options['projection'][$key]=true;
	foreach(array_keys((array)$areaboite) as $key) $options['projection'][$key]=true;
	$options['projection']['chemin']=true;

	// Requête pour récupérer l'ensemble des champs de la table metadata pour ce jeu de donnée
	$filter = strlen($id_metadata)>0 ? [ 'title' => $id_metadata ] : array();
	$obj = dbQueryToArray($filter, $options);
	$ret = count($obj);
	if ($ret > 0 ) {
		$ret = $obj;
	} else {
		$ret=0;
	}
	return $ret;
}


//----------------------------------------
// Get the datestamp corresponding to the record
//----------------------------------------

function getDatestamp ($record)
{
	$path=$record['chemin'];
	$json_file = str_replace('pgd_data','mnt',$path);
	$date = (file_exists($json_file)) ? date('Y-m-d', filectime($json_file)) : '2023-01-01' ;
	return $date;
}

//----------------------------------------
// Get the XML Dubin Core corresponding to the record
//----------------------------------------

function get_dcterms ($def, $map, $record)
{
	global $cvdir, $OAI_BASEURL;

	//----------------------------------------
	// Preparation / Initialization
	//----------------------------------------

	// Index of columns
	$CF=0; $CT=1; $VT=2; $VS=3; $CI=4; $PR=5;

	// Types of values
	$ValueTypes = array('#VALUE', '#DICO', '#ONTO');

	// Read dictionnaries and ontlogy definitions
	$cvlist=array();  // the corresponding cvlist for each dataverse field if applicable
	$cv=array();
	$cnt=1;
	while (strlen($def[$cnt][1] ?? '')>0) {
		$arr=$def[$cnt];
		if (! in_array( $arr[$CF], array_keys($record))) { $cnt++; continue; }
		// If there is a dico
		if (strlen($arr[$VS] ?? '')>0 && preg_match('/^cvlist:([a-zA-Z0-9_]+)/',$arr[$VS], $matches)) {
			$dico=$matches[1];
			if (! in_array($dico, $cvlist)) {
				${$dico} = tsvToArray($cvdir.$dico.'/'.$dico.'.txt');
				array_push($cvlist, $dico);
			}
		}
		// If there is a thesaurus / ontology
		if (strlen($arr[$VS] ?? '')>0 && preg_match('/^cv:([a-zA-Z0-9_]+)/',$arr[$VS], $matches)) {
			$dico=$matches[1];
			if (! in_array($dico, $cv)) {
				$idx=find_mapping($map, $dico);
				if ($idx>-1) ${$dico} = $map[$idx];
				array_push($cv, $dico);
			}
		}
		$cnt++;
	}

	// Get Metadata
	$path=$record['chemin'];
	$json_file = str_replace('pgd_data','mnt',$path);
	$date = (file_exists($json_file)) ? date('Y-m-d', filectime($json_file)) : '2023-01-01' ;
	$json = file_get_contents($json_file);
	$metadata = json_decode($json, true);

	//----------------------------------------
	// Parse the OAI-PMH definition file
	//----------------------------------------

	$xml = ''; $person = $subject = array();
	for($k=1; $k<count($def); $k++) {
		$arr = $def[$k];
		$field = $arr[$CF];
		if ($field !== 'resources') {
			if ( ! array_key_exists($field, $record)) continue;
			if (! is_array($record[$field])) {
				$value = $record[$field];
				if (in_array($arr[$CT], array('dc:title','dc:description')))
					$xml .= "\n".'<'.$arr[$CT].'><![CDATA['.$value.']]></'.$arr[$CT].'>';
				else
					$xml .= "\n".'<'.$arr[$CT].'>'.$value.'</'.$arr[$CT].'>';
				continue;
			}
			for($i=0; $i<count($record[$field]); $i++) {
				$ok = 1;
				$value = $record[$field][$i];
				if (in_array($arr[$CT], array('dc:creator','dc:contributor'))) {
					$value = str_replace(' ',', ',$value);
					if(! in_array($value, $person))
						$person[] = $value;
					else
						$ok=0;
				}
				if ($arr[$CT] == 'dc:subject')
					if(! in_array($value, $subject))
						$subject[] = $value;
					else
						$ok=0;
				if ($ok) {
					if ($arr[$VT] == '#DICO') {
						if (strlen($arr[$VS])>0 && preg_match('/^cvlist:([a-zA-Z0-9_]+)/',$arr[$VS], $matches)) {
							$dico=$matches[1];
							$opt = ($dico == 'people') ? array(0,' ',1) : array(0);
							$idx=find_dico(${$dico}, $record[$field][$i], $opt);
							if ($idx>-1 && strlen(${$dico}[$idx][$arr[$CI]])>0)
								$value = $arr[$PR].${$dico}[$idx][$arr[$CI]];
						}
					}
					if ($arr[$VT] == '#ONTO') {
						if (strlen($arr[$VS])>0 && preg_match('/^cv:([a-zA-Z0-9_]+)/',$arr[$VS], $matches)) {
							$dico=$matches[1];
							$out = get_term_info(${$dico}, $value);
							if (! empty($out) && strlen($out[$arr[$CI]])>0)
								$value = $out[$arr[$CI]];
						}
					}
					$xml .= "\n".'<'.$arr[$CT].'>'.$value.'</'.$arr[$CT].'>';
				}
			}
			continue;
		}
		$xml .= "\n".'<dc:identifier>'.dirname($OAI_BASEURL).'/metadata/'.$record['title'].'</dc:identifier>';
		if (isset($metadata['resources'])) {
			$res = $metadata['resources'];
			for($j=0; $j<count($res); $j++) {
				if (! isset($res[$j][$arr[$VS]])) continue;
				$key = $arr[$CT];
				$value = $res[$j][$arr[$VS]];
				if (strlen($value)==0 || ! preg_match('/^http*/', $value)) continue;
				$xml .= "\n".'<'.$key.'>'.$value.'</'.$key.'>';
			}
		}
	}
	$xml .= "\n".'<dc:date>'.$date.'</dc:date>';

	return $xml;
}

//----------------------------------------
// Get & check query parameters
//----------------------------------------

function get_params ($PARAM)
{
	global $OAI_DATE, $OAI_SET;
	$paramSet = array('identifier', 'metadataPrefix', 'set', 'from', 'until', 'resumptionToken', 'verb');
	$params = array('error' => 0, 'errorXML' => '');
	do
	{
		foreach ($PARAM as $key => $value)
			if (in_array($key, $paramSet))
				$params[$key] = $value;
			else {
				$params['errorXML'] = "\n".'<error code="badArgument">The argument &#039;'.$key.'&#039; (value=&#039;'.$value.'&#039;) included in the request is not valid.</error>';
				break;
			}
		if (strlen($params['errorXML']))
			break;
		if (! isset($params['verb'])) {
			echo "\n".'<error code="badVerb">The request does not provide any verb.</error>';
			break;
		}

		if (isset($params['resumptionToken'])) {
			$params['errorXML'] = "\n".'<error code="badResumptionToken">The value of the resumptionToken argument is invalid or expired.</error>';
			break;
		}

		if (in_array($params['verb'], array('ListIdentifiers','ListRecords','GetRecord')) && ! isset($params['metadataPrefix'])) {
			$params['errorXML'] = "\n".'<error code="badArgument">The required argument &#039;metadataPrefix&#039; is missing in the request.</error>';
			break;
		}

		if (isset($params['metadataPrefix']) && $params['metadataPrefix'] !== 'oai_dc') {
			$params['errorXML'] = "\n".'<error code="cannotDisseminateFormat">The metadata format &#039;'.$params['metadataPrefix'].'&#039; given by metadataPrefix is not supported by this repository.</error>';
			break;
		}

		if (in_array($params['verb'], array('Identify','ListSets','ListMetadataFormats','GetRecord')) && 
			(isset($params['set']) || isset($params['from']) || isset($params['until']))) {
			$params['errorXML'] = "\n".'<error code="badArgument">Some arguments included in the request are not valid.</error>';
			break;
		}

		if (in_array($params['verb'], array('Identify','ListSets','ListMetadataFormats')) && 
			(isset($params['set']) || isset($params['from']) || isset($params['until']))) {
			$params['errorXML'] = "\n".'<error code="badArgument">Some arguments included in the request are not valid.</error>';
			break;
		}

		if (! in_array($params['verb'], array('GetRecord','ListMetadataFormats')) && isset($params['identifier'])) {
			$params['errorXML'] = "\n".'<error code="badArgument">The argument &#039;identifier&#039; (value=&#039;'.$params['identifier'].'&#039;) included in the request is not valid.</error>';
			break;
		}

		if ($params['verb'] == 'GetRecord' && ! isset($params['identifier'])) {
			$params['errorXML'] = "\n".'<error code="badArgument">The argument &#039;identifier&#039; is missing in the request.</error>';
			break;
		}

		if (isset($params['identifier']) && ! preg_match('/^oai:[^:]+::([a-zA-Z0-9_]+)/',$params['identifier'])) {
			$params['errorXML'] = "\n".'<error code="badArgument">The argument &#039;identifier&#039; have an illegal syntax.</error>';
			break;
		}

		if (isset($params['set']) && $params['set'] !== $OAI_SET) {
			$params['errorXML'] = "\n".'<error code="badArgument">The argument &#039;set&#039; have an illegal value.</error>';
			break;
		}
		
		if ((isset($params['until']) && ! preg_match('/^[0-9][0-9][0-9][0-9]-[0-9][0-9]-[0-9][0-9]$/', $params['until'])) ||
		    (isset($params['from']) && ! preg_match('/^[0-9][0-9][0-9][0-9]-[0-9][0-9]-[0-9][0-9]$/', $params['from'])))  {
			$params['errorXML'] = "\n".'<error code="badArgument">The date argument provided is incorrect.</error>';
			break;
		}

		if (isset($params['from']) && isset($params['until']) && $params['from']>$params['until']) {
			$params['errorXML'] = "\n".'<error code="badArgument">The date argument provided is incorrect.</error>';
			break;
		}

	} while(0);

	if (strlen($params['errorXML']))
		$params['error']=1;

	return($params);
}

//----------------------------------------
// Main
//----------------------------------------

// verb given as a query parameter (GET)
if( isset( $_GET['verb'] ) ){
    $Verb =  $_GET['verb'];
	$PARAM = $_GET;
} else if (isset( $_POST['verb'] ) ){
    $Verb =  $_POST['verb'];
	$PARAM = $_POST;
}


$OAI_DC_HEADER = 'xmlns:oai_dc="http://www.openarchives.org/OAI/2.0/oai_dc/"
       xmlns:dc="http://purl.org/dc/elements/1.1/"
       xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
       xsi:schemaLocation="http://www.openarchives.org/OAI/2.0/oai_dc/ http://www.openarchives.org/OAI/2.0/oai_dc.xsd">';

// Header
header('Content-Type: text/xml');
echo '<?xml-stylesheet type="text/xsl" href="xsl/oai.xsl" ?>
<OAI-PMH xmlns="http://www.openarchives.org/OAI/2.0/"
       xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
       xsi:schemaLocation="http://www.openarchives.org/OAI/2.0/ http://www.openarchives.org/OAI/2.0/OAI-PMH.xsd">
<responseDate>'.str_replace(' ','T',date("Y-m-d H:i:s")).'Z</responseDate>';

do
{
	// Read OAI-PMH definition file
	if (! is_file($oaipmh_conf)) {
		echo '<error code="badDefinitionFile">'.$oaipmh_conf.' does not exist !!!</error>';
		break;
	}
	$def = tsvToArray($oaipmh_conf, false);
	if (! count($def)) {
		echo "\n".'<error code="badDefinitionFile">'.$oaipmh_conf.' is empty !!!</error>';
		break;
	}

	// Read Mapping definition file
	if (! is_file($mapping_conf)) {
		echo "\n".'<error code="badDefinitionFile">'.$mapping_conf.' does not exist !!!</error>';
		break;
	}
	$map = tsvToArray($mapping_conf, true);
	if (! count($map)) {
		echo "\n".'<error code="badDefinitionFile">'.$mapping_conf.' is empty !!!</error>';
		break;
	}

	//----------------------------------------
	// BadVerb
	//----------------------------------------
	if (! in_array($Verb, array('Identify', 'ListMetadataFormats', 'ListSets', 'ListRecords', 'ListIdentifiers', 'GetRecord'))) {
		echo "\n".'<error code="badVerb">The request does not provide any verb.</error>';
		break;
	}

	echo "\n".'<request verb="'.$Verb.'">'.$OAI_BASEURL.'</request>';

	// Get parameters
	$par = get_params ($PARAM);
	if ($par['error']) { echo $par['errorXML']; break; }


	//----------------------------------------
	// Identify
	//----------------------------------------
	if ($Verb == 'Identify') {
		echo '
  <Identify>
  <repositoryName>Maggot metadata repository</repositoryName>
  <baseURL>'.$OAI_BASEURL.'</baseURL>
  <protocolVersion>2.0</protocolVersion>
  <adminEmail>'.$OAI_EMAIL.'</adminEmail>
  <earliestDatestamp>'.$OAI_DATE.'</earliestDatestamp>
  <deletedRecord>no</deletedRecord>
  <granularity>YYYY-MM-DD</granularity>
  <description>
  <oai-identifier xmlns="http://www.openarchives.org/OAI/2.0/oai-identifier"
             xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
             xsi:schemaLocation="http://www.openarchives.org/OAI/2.0/oai-identifier http://www.openarchives.org/OAI/2.0/oai-identifier.xsd">
            <scheme>oai</scheme>
            <repositoryIdentifier>'.$OAI_IDENTIFIER.'</repositoryIdentifier>
            <delimiter>:</delimiter>
            <sampleIdentifier>oai:'.$OAI_IDENTIFIER.'::frim1</sampleIdentifier>
  </oai-identifier>
  </description>
  </Identify>';
		break;
	}


	//----------------------------------------
	// ListMetadataFormats
	//----------------------------------------
	if ($Verb == 'ListMetadataFormats') {
		echo '
  <ListMetadataFormats>
  <metadataFormat>
  <metadataPrefix>oai_dc</metadataPrefix>
  <schema>http://www.openarchives.org/OAI/2.0/oai_dc.xsd</schema>
  <metadataNamespace>http://www.openarchives.org/OAI/2.0/oai_dc/</metadataNamespace>
  </metadataFormat>
  </ListMetadataFormats>';
	}


	//----------------------------------------
	// ListSets
	//----------------------------------------
	if ($Verb == 'ListSets') {
		echo '
  <ListSets>
  <set>
  <setSpec>'.$OAI_SET.'</setSpec>
  <setName>Public Datasets</setName>
  <setDescription>
  <oai_dc:dc '.$OAI_DC_HEADER.'
       <dc:description>Public access rights</dc:description>
  </oai_dc:dc>
  </setDescription>
  </set>
  </ListSets>';
	}


	//----------------------------------------
	// ListIdentifiers
	//----------------------------------------
	if ($Verb == 'ListIdentifiers') {
		$from = isset($par['from']) ? $par['from'] : $OAI_DATE;
		$until = isset($par['until']) ? $par['until'] : date("Y-m-d");
		$records = getMetadata ($id_metadata);
		$LIST = '';
		if (is_array($records))
			for ($k=0; $k<count($records); $k++) {
				$record = (array)($records[$k]);
				if (is_accessible ($record)) continue;
				$datestamp = getDatestamp($record);
				if ($datestamp>=$from && $datestamp<=$until)
					$LIST .= "\n".'<header>'."\n".'<identifier>oai:'.$OAI_IDENTIFIER.'::'.$record['title'].'</identifier>'."\n".'<datestamp>'.$datestamp.'</datestamp>'."\n".'<setSpec>maggot</setSpec>'."\n".'</header>';
			}
		if (strlen($LIST))
			echo "\n".'<ListIdentifiers>'.$LIST."\n".'</ListIdentifiers>';
		else
			echo '<error code="noRecordsMatch">The combination of the values of the from, until, set and metadataPrefix arguments results in an empty list.</error>';
	}


	//----------------------------------------
	// ListRecords
	//----------------------------------------
	if ($Verb == 'ListRecords') {
		$from = isset($par['from']) ? $par['from'] : $OAI_DATE;
		$until = isset($par['until']) ? $par['until'] : date("Y-m-d");
		$records = getMetadata ();

		$LIST = '';
		if (is_array($records)) for ($k=0; $k<count($records); $k++) {
			$record = (array)($records[$k]);
			if (is_accessible ($record)) continue;
			$datestamp = getDatestamp($record);
			if ($datestamp>=$from && $datestamp<=$until) {
				$LIST .= "\n".'<record id="'.$record['title'].'">'."\n".'<header>'."\n".'<identifier>oai:'.$OAI_IDENTIFIER.'::'.$record['title'].'</identifier>'."\n".'  <datestamp>'.$datestamp.'</datestamp>'."\n".'<setSpec>maggot</setSpec>'."\n".'</header>'."\n".'<metadata>'."\n".'<oai_dc:dc '.$OAI_DC_HEADER;
//echo "<br><pre>"; print_r($record); echo "</pre><br>";
				$LIST .= get_dcterms($def,$map,$record);
				$LIST .= "\n".'</oai_dc:dc>'."\n".'</metadata></record>';
			}
		}
		if (strlen($LIST))
			echo "\n".'<ListRecords>'.$LIST."\n".'</ListRecords>';
		else
			echo '<error code="noRecordsMatch">The combination of the values of the from, until, set and metadataPrefix arguments results in an empty list.</error>';
	}


	//----------------------------------------
	// GetRecord
	//----------------------------------------
	if ($Verb == 'GetRecord') {
		$identifier = isset($PARAM['identifier']) ? $PARAM['identifier'] : '';
		$id_metadata = preg_match('/^oai:[^:]+::([a-zA-Z0-9_]+)/',$identifier, $matches) ? $matches[1] : '';
		$records = getMetadata ($id_metadata);
		if (is_array($records))
			$record = (array)($records[0]);
		else {
			echo "\n".'<error code="badArgument">The argument &#039;identifier&#039; (value=&#039;'.$identifier.'&#039;) included in the request is not valid.</error>';
			break;
		}
		if (is_accessible ($record)) {
			echo "\n".'<error code="badArgument">The argument &#039;identifier&#039; (value=&#039;'.$identifier.'&#039;) included in the request is not valid.</error>';
			break;
		}
		$datestamp = getDatestamp($record);
		echo "\n".'<GetRecord>'."\n".'<record id="'.$id_metadata.'">'."\n".'<header>'."\n".'<identifier>'.$identifier.'</identifier>'."\n".'<datestamp>'.$datestamp.'</datestamp>'."\n".'<setSpec>maggot</setSpec>'."\n".'</header>'."\n".'<metadata>'."\n".'<oai_dc:dc '.$OAI_DC_HEADER;
		echo get_dcterms($def,$map,$record);
		echo "\n".'</oai_dc:dc>'."\n".'</metadata>'."\n".'</record>'."\n".'</GetRecord>';
	}

} while(0);

echo "\n</OAI-PMH>";

?>
