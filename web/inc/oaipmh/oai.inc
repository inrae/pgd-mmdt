<?php
#
# OAI-PMH implementation 
# Preliminary work, in progress (sept 2023)
#
## See OAI-PMH Specifications version 2
## http://www.openarchives.org/OAI/openarchivesprotocol.html
#

$HTTP = isset($_SERVER['HTTPS']) ? 'https' : 'http';
$BASEURL=$HTTP.'://'.$APPURL.'/oai';
$EMAIL='pfmb-bordeaux@inra.fr';
$IDENTIFIER='pmb-bordeaux.fr';

$XMLHEADER = '<?xml-stylesheet type="text/xsl" href="inc/oaipmh/oai.xsl" ?>
<OAI-PMH xmlns="http://www.openarchives.org/OAI/2.0/"
         xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
         xsi:schemaLocation="http://www.openarchives.org/OAI/2.0/
         http://www.openarchives.org/OAI/2.0/OAI-PMH.xsd">
<responseDate>'.date("Y-m-d H:i:s T").'</responseDate>
';

// verb given as a query parameter (GET)
$Verb = 'Identify';
if( isset( $_GET['verb'] ) ){
    $Verb =  $_GET['verb'];
}

do {

	if ($Verb=='Identify') {
		header('Content-Type: text/xml');
		echo $XMLHEADER.'<request verb="Identify">'.$BASEURL.'</request>
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
             xsi:schemaLocation="http://www.openarchives.org/OAI/2.0/oai-identifier
             http://www.openarchives.org/OAI/2.0/oai-identifier.xsd">            <scheme>oai</scheme>
            <repositoryIdentifier>'.$IDENTIFIER.'</repositoryIdentifier>
            <delimiter>:</delimiter>
            <sampleIdentifier>oai:'.$IDENTIFIER.'::frim1</sampleIdentifier>
        </oai-identifier>
        </description>
 </Identify>
</OAI-PMH>';
		break;
	}

	if ($Verb=='ListMetadataFormats') {
		header('Content-Type: text/xml');
		echo $XMLHEADER.'<request verb="ListMetadataFormats">'.$BASEURL.'</request>
 <ListMetadataFormats>
  <metadataFormat>
   <metadataPrefix>oai_dc</metadataPrefix>
   <schema>http://www.openarchives.org/OAI/2.0/oai_dc.xsd</schema>
   <metadataNamespace>http://www.openarchives.org/OAI/2.0/oai_dc/</metadataNamespace>
  </metadataFormat>
 </ListMetadataFormats>
</OAI-PMH>';
		break;
	}

	if ($Verb=='ListRecords') {
		break;
	}

	if ($Verb=='ListSets') {
		header('Content-Type: text/xml');
		echo $XMLHEADER.'<request verb="ListSets">'.$BASEURL.'</request>
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
  </ListSets>
</OAI-PMH>';
		break;
	}

	if ($Verb=='ListIdentifiers') {
		break;
	}

} while(0);

?>
