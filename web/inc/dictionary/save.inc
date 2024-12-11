<?php

do {
	if ($USER !== $ADMINGROUP) {
		$msg = 'ERROR Only the admin account is allowed to save !!!';
		break;
	}

	if (! isset($_POST['dico'])) {
		$msg = 'ERROR the dictionary must be specified !!!';
		break;
	}
	$dico=$_POST['dico'];

	$dico_file=$cvdir.$dico.'/'.$dico.'.txt';
	if (! is_file($dico_file)) {
		$msg ='ERROR: '.$dico_file.' does not exist !!!';
		break;
	}
	if (! is_writable($dico_file)) {
		$msg ='ERROR: '.$dico_file.' is not writable !!!';
		break;
	}

	if (! isset($_POST['array'])) {
		$msg .= 'ERROR the dictionary array must be specified !!!';
		break;
	}

	$arr_dico = json_decode($_POST['array'],true);
	//print_r($arr_dico);

	$data_txt='';
	foreach($arr_dico as $arr) {
		$data_txt .= implode("\t",$arr)."\n";
	}
	//print_r($data_txt);

	try {
		$string_encoded = iconv( mb_detect_encoding( $data_txt ), 'Windows-1252//TRANSLIT', $data_txt );
		$file = fopen($dico_file, "w");
		fwrite($file, $string_encoded);
		fclose($file);
	} catch (Exception $e) {
		$msg = 'ERROR: Backup of the '.$dico. ' dictionary failed';
	}

	$msg = 'Successful saving of the '.$dico. ' dictionary';

} while(0);

echo $msg;

?>
