<?php

// Retrieve all CVs from the "biomedical" block
function get_biomedicalCV ()
{
    global $dataverse_urls;
	$SERVER_URL= $dataverse_urls[0].'/api/v1';
    $schema = file_get_contents($SERVER_URL."/metadatablocks/biomedical");
    $arrCV = array();
    if ($schema) {
         $json = json_decode($schema);
         $arrfields = (array)$json->data->fields;
         $fields = array_keys($arrfields);
         foreach($fields as $field) {
             $arr = (array)$arrfields[$field];
             if (empty($arr['isControlledVocabulary'])) continue;
             $arrCV[$field] = $arr['controlledVocabularyValues'];
         }
    }
    return $arrCV;
}


////////////////////////////////////
// biomedical block
////////////////////////////////////

// Return the corresponding Dataverse JSON string for the field $field
// Distinction between the two types of vocabularies (predefined CVs and others)
// ensuring that the fields in the Dataverse metadata record are correctly populated. 
function getJSONfield($field, $fieldOther, $vocab)
{
    global $biomedicalCV;
    $json = '';
	if (empty($biomedicalCV)) return '';

    $fields = array_keys($biomedicalCV);
    $arrCV = array();
    $arrNoCV = array();
    if (in_array($field, $fields)) {
        foreach($vocab as $value) {
            if (in_array(strtolower($value), array_map('strtolower', $biomedicalCV[$field]))) {
                $idx = array_search(strtolower($value), array_map('strtolower', $biomedicalCV[$field]));
                array_push($arrCV, $biomedicalCV[$field][$idx]);
            } else {
                array_push($arrNoCV, $value);
            }
        }
        $sep='';
        if (count($arrNoCV)) {
            array_push($arrCV, 'Other');
            $sep =',';
        }
        if (count($arrCV))
            $json .= cvList($field, $arrCV);
        if (count($arrNoCV))
            $json .= $sep.simpleArray($fieldOther,$arrNoCV);
    }

//if (count($arrCV))   { echo "<br>--------- $field --------------<br><pre>"; print_r($arrCV); echo "</pre><br>";
//if (count($arrNoCV)) { echo "--------- $fieldOther -------------<br><pre>"; print_r($arrNoCV); echo "</pre><br>"; } 
//echo "$json <br>"; 

    return $json;
}

// Design
function getfield_DesignType($metadata, $df, $dico)
{
    return getJSONfield('studyDesignType', 'studyDesignTypeOther', $metadata[ $df[0] ]);
}

// Organism
function getfield_Organism($metadata, $df, $dico)
{
    return getJSONfield('studyAssayOrganism', 'studyAssayOtherOrganism', $metadata[ $df[0] ]);
}

// Factor
function getfield_FactorType($metadata, $df, $dico)
{
    return getJSONfield('studyFactorType', 'studyFactorTypeOther', $metadata[ $df[0] ]);
}

// Measurement
function getfield_MeasurementType($metadata, $df, $dico)
{
    return getJSONfield('studyAssayMeasurementType', 'studyAssayOtherMeasurmentType', $metadata[ $df[0] ]);
}

// Technology
function getfield_TechnologyType($metadata, $df, $dico)
{
    return getJSONfield('studyAssayTechnologyType', 'studyAssayTechnologyTypeOther', $metadata[ $df[0] ]);
}

// Platform
function getfield_Platform($metadata, $df, $dico)
{
    return getJSONfield('studyAssayPlatform', 'studyAssayPlatformOther', $metadata[ $df[0] ]);
}

// Cell
function getfield_CellType($metadata, $df, $dico)
{
    return simpleArray('studyAssayCellType',$metadata[ $df[0] ]);
}

// Sample
function getfield_SampleType($metadata, $df, $dico)
{
    return simpleArray('studySampleType',$metadata[ $df[0] ]);
}

// Protocol
function getfield_ProtocolType($metadata, $df, $dico)
{
    return simpleArray('studyProtocolType',$metadata[ $df[0] ]);
}

?>
