<?php
#
# OAI-PMH very quick (& dirty) implementation 
# Preliminary work, in progress (sept 2023)
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

$HTTP = 'http';
$BASEURL=$HTTP.'://'.$APPURL.'/oai';
$EMAIL='pfmb-bordeaux@inra.fr';
$IDENTIFIER='pmb-bordeaux.fr';

// verb given as a query parameter (GET)
if( isset( $_GET['verb'] ) ){
    $Verb =  $_GET['verb'];
}

//echo "$BASEURL - $EMAIL - $IDENTIFIER - $Verb<br>"; break;

do
{

	if (! in_array($Verb, array('Identify', 'ListMetadataFormats', 'ListSets', 'ListRecords', 'ListIdentifiers')))
		break;

	// Header
	header('Content-Type: text/xml');
	echo '<?xml-stylesheet type="text/xsl" href="inc/oaipmh/oai.xsl" ?>
  <OAI-PMH xmlns="http://www.openarchives.org/OAI/2.0/"
         xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
         xsi:schemaLocation="http://www.openarchives.org/OAI/2.0/
         http://www.openarchives.org/OAI/2.0/OAI-PMH.xsd">
  <responseDate>'.date("Y-m-d H:i:s T").'</responseDate><request verb="'.$Verb.'">'.$BASEURL.'</request>';


	// Identify
	if ($Verb == 'Identify') {
		echo '
  <Identify>
  <repositoryName>Maggot metadata repository</repositoryName>
  <baseURL>'.$BASEURL.'</baseURL>
  <protocolVersion>2.0</protocolVersion>
  <adminEmail>'.$EMAIL.'</adminEmail>
  <earliestDatestamp>2023-01-01</earliestDatestamp>
  <deletedRecord>no</deletedRecord>
  <granularity>YYYY-MM-DD</granularity>
  <description>
  <oai-identifier xmlns="http://www.openarchives.org/OAI/2.0/oai-identifier"
             xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
             xsi:schemaLocation="http://www.openarchives.org/OAI/2.0/oai-identifier             http://www.openarchives.org/OAI/2.0/oai-identifier.xsd">
			 <scheme>oai</scheme>
            <repositoryIdentifier>'.$IDENTIFIER.'</repositoryIdentifier>
            <delimiter>:</delimiter>
            <sampleIdentifier>oai:'.$IDENTIFIER.'::frim1</sampleIdentifier>
  </oai-identifier>
  </description>
  </Identify>';
	}

	// ListMetadataFormats
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

	// ListSets
	if ($Verb == 'ListSets') {
		echo '
  <ListSets>
  <set>
  <setSpec>maggot</setSpec>
  <setName>Public</setName>
  <setDescription>
  <oai_dc:dc
       xmlns:oai_dc="http://www.openarchives.org/OAI/2.0/oai_dc/"
       xmlns:dc="http://purl.org/dc/elements/1.1/"
       xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
       xsi:schemaLocation="http://www.openarchives.org/OAI/2.0/oai_dc/
       http://www.openarchives.org/OAI/2.0/oai_dc.xsd">
       <dc:description>Public access rights</dc:description>
  </oai_dc:dc>
  </setDescription>
  </set>
  <set>
  <setSpec>maggot</setSpec>
  <setName>Private</setName>
  <setDescription>
  <oai_dc:dc
       xmlns:oai_dc="http://www.openarchives.org/OAI/2.0/oai_dc/"
       xmlns:dc="http://purl.org/dc/elements/1.1/"
       xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
       xsi:schemaLocation="http://www.openarchives.org/OAI/2.0/oai_dc/
       http://www.openarchives.org/OAI/2.0/oai_dc.xsd">
       <dc:description>Private access rights</dc:description>
  </oai_dc:dc>
  </setDescription>
  </set>
  <set>
  <setSpec>maggot</setSpec>
  <setName>Mixte</setName>
  <setDescription>
  <oai_dc:dc
       xmlns:oai_dc="http://www.openarchives.org/OAI/2.0/oai_dc/"
       xmlns:dc="http://purl.org/dc/elements/1.1/"
       xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
       xsi:schemaLocation="http://www.openarchives.org/OAI/2.0/oai_dc/
       http://www.openarchives.org/OAI/2.0/oai_dc.xsd">
       <dc:description>Mixte access rights</dc:description>
  </oai_dc:dc>
  </setDescription>
  </set>
  </ListSets>';
	}

	// ListRecords
	if ($Verb == 'ListRecords') {
	}

	// ListIdentifiers
	if ($Verb == 'ListIdentifiers') {
	}

	echo '</OAI-PMH>';

} while(0);

?>
