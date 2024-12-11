<?php

$config_file = './conf/config_terms.json';
$schema_file = './conf/maggot-schema.json';

$OK=0;

// Traitement de la demande
do {
	if ($USER !== $ADMINGROUP) {
		echo "<p style=\"color:red\">ERROR: Only the admin account is allowed to copy</p><br>";
		break;
	}
	
	// Get the KEY
	if( ! isset( $_GET['key'] ) ) {
		echo "<p style=\"color:red\">ERROR: The key is not passed</p><br>";
		break;
	}
	$KEY = $_GET['key'];
	
	// Test if generated files exist 
	$config_terms = './tmp/'.$KEY.'.json';
	if (!file_exists($config_terms)) {
		echo "<p style=\"color:red\">ERROR: ".$config_terms." file does not exists</p><br>";
		break;
	}
	$json_schema = './tmp/'.$KEY.'-schema.json';
	if (!file_exists($config_terms)) {
		echo "<p style=\"color:red\">ERROR: ".$json_schema." file does not exists</p><br>";
		break;
	}
	
	// Test if original files are writable
	if (!is_writable($config_file)) {
		echo "<p style=\"color:red\">ERROR: ".$config_file." file is not writable</p><br>";
		break;
	}
	if (!is_writable($schema_file)) {
		echo "<p style=\"color:red\">ERROR: ".$schema_file." file is not writable</p><br>";
		break;
	}
	
	// Save the original files
	if (!rename($config_file, $config_file.'.old')) {
		echo "<p style=\"color:red\">ERROR: ".basename($config_file)." file cannot be renamed</p><br>";
		break;
	}
	if (!rename($schema_file, $schema_file.'.old')) {
		echo "<p style=\"color:red\">ERROR: ".basename($schema_file)." file cannot be renamed</p><br>";
		break;
	}
	
	// Copy the generated files to original files
	if (!copy($config_terms, $config_file)) {
		echo "<p style=\"color:red\">ERROR: the copy of the ".basename($config_file)." file failed !!</p><br>";
		break;
	}
	if (!copy($json_schema, $schema_file)) {
		echo "<p style=\"color:red\">ERROR: the copy of the ".basename($schema_file)." file failed !!</p><br>";
		break;
	}
	
	// Success
	echo "<p style=\"color:green\">SUCCESS: the files have been copied successfully</p>";
	echo "<p style=\"color:orange\">Warning: You must now restart the application in 'fullstart' mode to effectively activate this new configuration.</p><br>";
	$OK=1;
	
} while (0);

if (!$OK) {
	echo "<p style=\"color:orange\">Warning: Due to rights issues, you have to download the JSON files and upload them to the right place yourself.</p><br>";
}

?>

