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
# Copyright: (c) INRAE - 2015-2023
#

// verb given as a query parameter (GET)
if( isset( $_GET['verb'] ) ){
    $Verb =  $_GET['verb'];
}

function getMetadata ($id_metadata='') {
	global $client, $txtboite, $listboite, $checkboite, $multiboite, $areaboite;
	
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
	$query = new MongoDB\Driver\Query($filter, $options);
	$obj = $client->executeQuery('pgd-db.metadata', $query)->toArray();
	$ret = count($obj);
	if ($ret > 0 ) {
		$ret = $obj;
	} else {
		$ret=0;
	}
	return $ret;
}

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
	while (strlen($def[$cnt][1])>0) {
		$arr=$def[$cnt];
		if (! in_array( $arr[$CF], array_keys($record))) { $cnt++; continue; }
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
		$res = $metadata['resources'];
		for($j=0; $j<count($res); $j++) {
			if (! isset($res[$j][$arr[$VS]])) continue;
			$key = $arr[$CT];
			$value = $res[$j][$arr[$VS]];
			if (strlen($value)==0 || ! preg_match('/^http*/', $value)) continue;
			$xml .= "\n".'<'.$key.'>'.$value.'</'.$key.'>';
		}
	}
	$xml .= "\n".'<dc:date>'.$date.'</dc:date>';

	return $xml;
}


//----------------------------------------
// Main
//----------------------------------------

do
{
	// Read OAI-PMH definition file
	if (! is_file($oaipmh_conf)) {
		$msg='Error: '.$oaipmh_conf.' does not exist !!!';
		break;
	}
	$def = tsvToArray($oaipmh_conf, false);
	if (! count($def)) {
		$msg='Error: '.$oaipmh_conf.' is empty !!!';
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

	//----------------------------------------
	// BadVerb
	//----------------------------------------
	if (! in_array($Verb, array('Identify', 'ListMetadataFormats', 'ListSets', 'ListRecords', 'ListIdentifiers', 'GetRecord')))
		echo '
<error code="badVerb">The request does not provide any verb.</error>';


	//----------------------------------------
	// Identify
	//----------------------------------------
	if ($Verb == 'Identify') {
		echo '
  <request verb="'.$Verb.'">'.$OAI_BASEURL.'</request>
  <Identify>
  <repositoryName>Maggot metadata repository</repositoryName>
  <baseURL>'.$OAI_BASEURL.'</baseURL>
  <protocolVersion>2.0</protocolVersion>
  <adminEmail>'.$OAI_EMAIL.'</adminEmail>
  <earliestDatestamp>2023-01-01</earliestDatestamp>
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
	}


	//----------------------------------------
	// ListMetadataFormats
	//----------------------------------------
	if ($Verb == 'ListMetadataFormats') {
		echo '
  <request verb="'.$Verb.'">'.$OAI_BASEURL.'</request>
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
  <request verb="'.$Verb.'">'.$OAI_BASEURL.'</request>
  <ListSets>
  <set>
  <setSpec>maggot</setSpec>
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
		echo '
  <request verb="'.$Verb.'" metadataPrefix="oai_dc">'.$OAI_BASEURL.'</request>';
  		
		if ((isset($_GET['until']) && ! preg_match('/^[0-9][0-9][0-9][0-9]-[0-9][0-9]-[0-9][0-9]/', $_GET['until'])) ||
		    (isset($_GET['from']) && ! preg_match('/^[0-9][0-9][0-9][0-9]-[0-9][0-9]-[0-9][0-9]/', $_GET['from'])) ||
		    (isset($_GET['resumptionToken']) && $_GET['resumptionToken'] == 'junk') ) {
			echo '
<error code="badArgument">The required argument &#039;identifier&#039; is missing in the request.</error>';
		} else {
			echo '
  <ListIdentifiers>';
		$records = getMetadata ($id_metadata);
		if (is_array($records))
			for ($k=0; $k<count($records); $k++)
				echo '
  <header>
   <identifier>oai:'.$OAI_IDENTIFIER.'::'.((array)($records[$k]))['title'].'</identifier>
   <datestamp>2023-09-29</datestamp>
   <setSpec>maggot</setSpec>
  </header>';
		echo '  
  </ListIdentifiers>';
		}
	}


	//----------------------------------------
	// ListRecords
	//----------------------------------------
	if ($Verb == 'ListRecords') {
		echo '
  <request verb="'.$Verb.'" metadataPrefix="oai_dc">'.$OAI_BASEURL.'</request>';
  
		if ((isset($_GET['until']) && ! preg_match('/^[0-9][0-9][0-9][0-9]-[0-9][0-9]-[0-9][0-9]/', $_GET['until'])) ||
		    (isset($_GET['from']) && ! preg_match('/^[0-9][0-9][0-9][0-9]-[0-9][0-9]-[0-9][0-9]/', $_GET['from'])) ||
		    (isset($_GET['resumptionToken']) && $_GET['resumptionToken'] == 'junk') ) {
			echo '
<error code="badArgument">The required argument &#039;identifier&#039; is missing in the request.</error>';
		} else {
			echo '
  <ListRecords>';
			$records = getMetadata ();
			if (is_array($records)) for ($k=0; $k<count($records); $k++) {
				$record = (array)($records[$k]);
				echo '
  <record id="'.$record['title'].'">
  <header>
   <identifier>oai:'.$OAI_IDENTIFIER.'::'.$record['title'].'</identifier>
   <datestamp>2023-09-29</datestamp>
   <setSpec>maggot</setSpec>
  </header>
  <metadata>
     <oai_dc:dc '.$OAI_DC_HEADER;
				echo get_dcterms($def,$map,$record);
				echo '
    </oai_dc:dc>
  </metadata>
  </record>';
			}
		echo '
  </ListRecords>';
		}
	}


	//----------------------------------------
	// GetRecord
	//----------------------------------------
	if ($Verb == 'GetRecord') {
		$identifier = isset($_GET['identifier']) ? $_GET['identifier'] : '';
		$id_metadata = preg_match('/^oai:[^:]+::([a-zA-Z0-9_]+)/',$identifier, $matches) ? $matches[1] : '';
		$record = getMetadata ($id_metadata);
		if (is_array($record))
			$record = (array)($record[0]);
		else
			$id_metadata = '';
 
		if (strlen($id_metadata)>0) {
			echo '
  <request verb="'.$Verb.'" metadataPrefix="oai_dc" identifier="'.$identifier.'">'.$OAI_BASEURL.'</request>
  <GetRecord>
  <record id="'.$id_metadata.'">
  <header>
   <identifier>'.$identifier.'</identifier>
   <datestamp>2023-09-29</datestamp>
   <setSpec>maggot</setSpec>
  </header>
  <metadata>
     <oai_dc:dc '.$OAI_DC_HEADER;
			echo get_dcterms($def,$map,$record);
			echo '
    </oai_dc:dc>
  </metadata>
  </record>
  </GetRecord>';
		} else {
			echo '
  <request verb="'.$Verb.'">'.$OAI_BASEURL.'</request>
  <error code="badArgument">The required argument &#039;identifier&#039; is missing in the request.</error>';
		}
	}

	echo '
  </OAI-PMH>';

} while(0);

?>
