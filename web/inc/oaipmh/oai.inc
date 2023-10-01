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

    $OAI_DC_HEADER = 'xmlns:oai_dc="http://www.openarchives.org/OAI/2.0/oai_dc/"
       xmlns:dc="http://purl.org/dc/elements/1.1/"
       xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
       xsi:schemaLocation="http://www.openarchives.org/OAI/2.0/oai_dc/ http://www.openarchives.org/OAI/2.0/oai_dc.xsd">';

	if (! in_array($Verb, array('Identify', 'ListMetadataFormats', 'ListSets', 'ListRecords', 'ListIdentifiers', 'GetRecord')))
		break;

	// Header
	header('Content-Type: text/xml');
	echo '<?xml-stylesheet type="text/xsl" href="xsl/oai.xsl" ?>
  <OAI-PMH xmlns="http://www.openarchives.org/OAI/2.0/"
         xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
         xsi:schemaLocation="http://www.openarchives.org/OAI/2.0/ http://www.openarchives.org/OAI/2.0/OAI-PMH.xsd">
  <responseDate>'.date("Y-m-d H:i:s T").'</responseDate>';


	// Identify
	if ($Verb == 'Identify') {
		echo '
  <request verb="'.$Verb.'">'.$BASEURL.'</request>
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
             xsi:schemaLocation="http://www.openarchives.org/OAI/2.0/oai-identifier http://www.openarchives.org/OAI/2.0/oai-identifier.xsd">
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
  <request verb="'.$Verb.'">'.$BASEURL.'</request>
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
  <request verb="'.$Verb.'">'.$BASEURL.'</request>
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

	// ListIdentifiers
	if ($Verb == 'ListIdentifiers') {
		echo '
  <request verb="'.$Verb.'" metadataPrefix="oai_dc">'.$BASEURL.'</request>
  <ListIdentifiers>
  <header>
   <identifier>oai:'.$IDENTIFIER.'::frim1</identifier>
   <datestamp>2023-09-29</datestamp>
   <setSpec>maggot</setSpec>
  </header>
  </ListIdentifiers>';
	}

	// ListRecords
	if ($Verb == 'ListRecords') {
		echo '
  <request verb="'.$Verb.'" metadataPrefix="oai_dc">'.$BASEURL.'</request>
  <ListRecords>
  <record>
  <header>
   <identifier>oai:'.$IDENTIFIER.'::frim1</identifier>
   <datestamp>2023-09-29</datestamp>
   <setSpec>maggot</setSpec>
  </header>
  <metadata>
     <oai_dc:dc '.$OAI_DC_HEADER.'
	  <dc:title><![CDATA[Friml]]></dc:title>
	  <dc:creator>Gibon Yves</dc:creator>
	  <dc:description><![CDATA[FRIM Fruit Integrative Modelling]]></dc:description>
	  <dc:date>2023-09-29</dc:date>
      <dc:identifier><![CDATA[https://pmb-bordeaux.fr/maggot/metadata/frim1]]></dc:identifier>
      <dc:language>eng</dc:language>
    </oai_dc:dc>
  </metadata>
  </record>
  </ListRecords>';
	}

	// GetRecord
	if ($Verb == 'GetRecord') {
		echo '
  <request verb="'.$Verb.'" metadataPrefix="oai_dc" identifier="oai:'.$IDENTIFIER.'::frim1">'.$BASEURL.'</request>
  <GetRecord>
  <record>
  <header>
   <identifier>oai:'.$IDENTIFIER.'::frim1</identifier>
   <datestamp>2023-09-29</datestamp>
   <setSpec>maggot</setSpec>
  </header>
  <metadata>
     <oai_dc:dc '.$OAI_DC_HEADER.'
	  <dc:title><![CDATA[Friml]]></dc:title>
	  <dc:creator>Gibon Yves</dc:creator>
	  <dc:description><![CDATA[FRIM Fruit Integrative Modelling]]></dc:description>
	  <dc:date>2023-09-29</dc:date>
      <dc:identifier><![CDATA[https://pmb-bordeaux.fr/maggot/metadata/frim1]]></dc:identifier>
    </oai_dc:dc>
  </metadata>
  </record>
  </GetRecord>';
	}

	echo '</OAI-PMH>';

} while(0);

?>
