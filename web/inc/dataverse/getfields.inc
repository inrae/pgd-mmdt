<?php

// ======================
// Functions generating the elementary structures of the JSON format of Dataverse
// ======================

function simpleField($field, $value)
{
	$value = $value ? str_replace('"', '\"', str_replace("\n", '', $value)) : '';
	return '{"typeName": "'.$field.'","multiple": false,"typeClass": "primitive","value": "'.$value.'"}';
}

function simpleArray($field, $value)
{
	$value = $value ? str_replace("\n", '', $value) : '';
	return '{"typeName": "'.$field.'","multiple": true,"typeClass": "primitive","value":['.$value.']}';
}

function compoundArray($field, $json)
{
	return '{"typeName": "'.$field.'","multiple": true,"typeClass": "compound","value":['.$json.']}';
}

function cvField($field, $value)
{
	return '{"typeName": "'.$field.'","multiple": false,"typeClass": "controlledVocabulary","value": "'.$value.'"}';
}

function cvArray($field, $json)
{
	return '{"typeName": "'.$field.'","multiple": true,"typeClass": "controlledVocabulary","value":['.$json.']}';
}

function cvList($field, $list)
{
	$json='';
	$sep='';
	foreach($list as $name) {
		$json.=$sep.'"'.$name.'"';
		$sep=',';
	}
	return cvArray($field, $json);
}


// ======================
// Functions generating the structures of the JSON format of Dataverse for each of the fields
// ======================

// title
function getfield_title($metadata, $df, $dico)
{
	return simpleField('title', $metadata[ $df[0] ]);
}

// notesText
function getfield_notesText($metadata, $df, $dico)
{
	return simpleField('notesText', $metadata[ $df[0] ]);
}

// otherId
function getfield_otherId($metadata)
{
	$json = '{"otherIdAgency": '.simpleField('otherIdAgency','Maggot').
			',"otherIdValue":'.simpleField('otherIdValue',otherID($metadata['title'])).'}';
	return compoundArray('otherId',$json);
}


// subject
function getfield_subject($metadata, $df, $dico)
{
	return str_replace('Medicine ','Medicine, ', cvList('subject',$metadata[ $df[0] ]));
}

// language
function getfield_language($metadata, $df, $dico)
{
	return cvList('language',$metadata[ $df[0] ]);
}

// kindOfData
function getfield_kindOfData($metadata, $df, $dico)
{
	return cvList('kindOfData',$metadata[ $df[0] ]);
}

// dataOrigin
function getfield_dataOrigin($metadata, $df, $dico)
{
	return cvList('dataOrigin',$metadata[ $df[0] ]);
}

// lifeCycleStep
function getfield_lifeCycleStep($metadata, $df, $dico)
{
	return cvList('lifeCycleStep',$metadata[ $df[0] ]);
}

// depositor
function getfield_depositor($metadata, $df, $dico)
{
	$json = simpleField('depositor', $metadata[ $df[0] ]);
	$json .= ','.simpleField('dateOfDeposit', date("Y-m-d"));
	return $json;
}

// datasetContact
function getfield_datasetContact($metadata, $df, $dico)
{
	$json='';
	$sep='';
	foreach($metadata[$df[0]] as $name) {
		$institute=$email='';
		$idx=find_dico($dico, $name, array(0,' ',1));
		if ($idx>-1) {
			$name=$dico[$idx][0].' '.$dico[$idx][1]; $institute=$dico[$idx][2]; $email=$dico[$idx][4];
		}
		$json.=$sep.'{"datasetContactName":'.simpleField('datasetContactName',$name);
		if (strlen($institute))
			$json.= ',"datasetContactAffiliation":'.simpleField('datasetContactAffiliation',$institute);
		if (strlen($email))
			$json.= ',"datasetContactEmail":'.simpleField('datasetContactEmail',$email);
		$json.='}';
		$sep=',';
	}
	return compoundArray('datasetContact',$json);
}

// author
function getfield_author($metadata, $df, $dico)
{
	$json='';
	$sep='';
	foreach($metadata[$df[0]] as $name) {
		$institute=$orcid='';
		$idx=find_dico($dico, $name, array(0,' ',1));
		if ($idx>-1) {
			$name=$dico[$idx][0].' '.$dico[$idx][1]; $institute=$dico[$idx][2]; $orcid=$dico[$idx][3];
		}
		$json.=$sep.'{"authorName":'.simpleField('authorName',$name);
		if (strlen($institute))
			$json.= ',"authorAffiliation":'.simpleField('authorAffiliation',$institute);
		if (strlen($orcid)>0)
		$json.=',"authorIdentifierScheme":'.cvField('authorIdentifierScheme','ORCID').
				',"authorIdentifier":'.simpleField('authorIdentifier',$orcid); 
		$json.='}';
		$sep=',';
	}
	return compoundArray('author',$json);
}

// contributor
function getfield_contributor($metadata, $df, $arr_dico)
{
	$json='';
	$sep='';
	$cnt=0;
	foreach($df as $arr) {
		$mkey=$arr[0]; $dkey=$arr[1]; $dico=$arr_dico[$cnt];
		$names = $metadata[ $mkey ];
		for($i=0; $i<count($names); $i++) {
			$name=$names[$i];
			$institute=$orcid='';
			$idx=find_dico($dico, $name, array(0,' ',1));
			if ($idx>-1) {
				$name=$dico[$idx][0].' '.$dico[$idx][1]; $institute=$dico[$idx][2]; $orcid=$dico[$idx][3];
			}
			$json.=$sep.'{"contributorType":'.cvField('contributorType',$dkey).
						',"contributorName":'.simpleField('contributorName',$name);
			if (strlen($institute)>0)
			$json.=',"contributorAffiliation":'.simpleField('contributorAffiliation',$institute);
			if (strlen($orcid)>0)
			$json.=',"contributorIdentifierScheme":'.cvField('contributorIdentifierScheme','ORCID').
					',"contributorIdentifier":'.simpleField('contributorIdentifier',$orcid);
			$json.='}';
			$sep=',';
		}
		$cnt++;
	}
	return compoundArray('contributor',$json);
}

// producer
function getfield_producer($metadata, $df, $dico)
{
	$json='';
	$sep='';
	foreach($metadata[$df[0]] as $name) {
		$institute=$email='';
		$idx=find_dico($dico, $name, array(0));
		if ($idx>-1) {
			$name=$dico[$idx][0]; $institute=$dico[$idx][1]; $url_producer=$dico[$idx][2]; $url_logo=$dico[$idx][3];
		}
		$json.= $sep.'{"producerName":'.simpleField('producerName',$name);
		if (strlen($institute))
			$json.= ',"producerAffiliation":'.simpleField('producerAffiliation',$institute);
		if (strlen($url_producer))
			$json.= ',"producerURL":'.simpleField('producerURL',$url_producer);
		if (strlen($url_logo))
			$json.= ',"producerLogoURL":'.simpleField('producerLogoURL',$url_logo);
		$json.='}';
		$sep=',';
	}
	return compoundArray('producer',$json);
}

// dsDescription
function getfield_dsDescription($metadata, $df, $dico)
{
	$json = '{"dsDescriptionValue":'.simpleField('dsDescriptionValue',$metadata[ $df[0] ]);
	$json.=',"dsDescriptionLanguage":'.cvField('dsDescriptionLanguage','English');
	$json.=',"dsDescriptionDate":'.simpleField('dsDescriptionDate',date("Y-m-d")).'}';
	return compoundArray('dsDescription',$json);
}

// publication
function getfield_publication($metadata, $df, $dico)
{
	$json='{';
	$sep='';
	$valid=0;
	foreach($df as $arr) {
		$val=$metadata[ $arr[0] ];
		if ($arr[1]==='publicationCitation') {
			$json.=$sep.'"publicationCitation":'.simpleField('publicationCitation',$val);
			$valid=1;
		}
		else if ($arr[1]==='publicationIDType')
			$json.=$sep.'"publicationIDType":'.cvField('publicationIDType',$val);
		else if ($arr[1]==='publicationIDNumber')
			$json.=$sep.'"publicationIDNumber":'.simpleField('publicationIDNumber',$val);
		else if ($arr[1]==='publicationURL')
			$json.=$sep.'"publicationURL":'.simpleField('publicationURL',$val);
		$sep=',';
	}
	$json .='}';
	if (!$valid) $json='';
	return compoundArray('publication',$json);
}

// grantNumber
function getfield_grantNumber($metadata, $df, $dico)
{
	$json='';
	$sep='';
	foreach($metadata[$df[0]] as $name) {
		$agence=$idagence='';
		$idx=find_dico($dico, $name, array(0));
		if ($idx>-1) {
			$agence=$dico[$idx][1]; $idagence=$dico[$idx][2];
		}
		$json.=$sep.'{"grantNumberAgency":'.simpleField('grantNumberAgency',$agence);
		$json.=',"grantNumberValue":'.simpleField('grantNumberValue',$idagence).'}';
		$sep=',';
	}
	return compoundArray('grantNumber',$json);
}

// keyword
function getfield_keyword($metadata, $df, $dico)
{
	$json='';
	$sep='';
	$out = array();
	foreach($metadata[$df[0]] as $name) {
		$json.=$sep.'{"keywordValue":'.simpleField('keywordValue',$name);
		$out = get_term_info($dico, $name);
		if (! empty($out)) {
			$json.=',"keywordVocabulary":'.simpleField('keywordVocabulary',$out[0]);
			$json.=',"keywordTermURL":'.simpleField('keywordTermURL',$out[1]);
		}
		$json .='}';
		$sep=',';
	}
	return compoundArray('keyword',$json);
}

// topicClassification
function getfield_topicClassification($metadata, $df, $dico)
{
	$json='';
	$sep='';
	$out = array();
	foreach($metadata[$df[0]] as $name) {
		$json.=$sep.'{"topicClassValue":'.simpleField('topicClassValue',$name);
		$out = get_term_info($dico, $name);
		if (! empty($out)) {
			$json.=',"topicClassVocab":'.simpleField('topicClassVocab',$out[0]);
			$json.=',"topicClassVocabURI":'.simpleField('topicClassVocabURI',$out[1]);
		}
		$json .='}';
		$sep=',';
	}
	return compoundArray('topicClassification',$json);
}

// otherReferences
function getfield_otherReferences($metadata, $df, $dico)
{
	$str='';
	$sep='';
	foreach($metadata[$df[0]] as $arr) {
		if (strlen($arr['location'])>0 && preg_match('/^https?:/',$arr['location'])) {
			$str .= $sep.'"'.$arr['datatype'].': '.$arr['description'].', '.$arr['location'].'"';
			$sep=',';
		}
	}
	return simpleArray('otherReferences',$str);
}

?>
