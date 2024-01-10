<?php

$ID = genKEY();
$crypt_url = 'tmp/'.$ID.'.txt';
$key = isset($_POST['key']) ? $_POST['key'] : '';
$keysession = isset($_POST['keysession']) ? $_POST['keysession'] : '';


?>

<!DOCTYPE html>
<html lang="en" >

<head>
	<title>Maggot - Encryption</title>
	<?php echo $HEAD ?>
</head>

<body style="zoom: <? echo $ZOOMWP ?>">

<?php echo $header_crypt ?>
<br><br>

<h1>Private access key management</h1>
<br>

<section>
	<br/>
	<form id="formconvert" name="formconvert" action="#" autocomplete="off" method="post" enctype="multipart/form-data">
		<h2>Generate a private access key file</h2>
		<span><b>Private Key</b> : <input class="input-setval " type="password" name="key" id="key" style="width:350px" placeholder="8 characters at least" value="<? echo $key ?>"></span>
		<input type="submit" class="btn btn-success btn-xs" style="width: 100px;" name="encrypt" value="Encryption" />
		<br><br>
		<input id="download" type="download" value="Download the Private Access File" class="btn btn-success btn-xs" style="width: 300px; display: none" onclick="downloadFile('<?php echo $PATH.$crypt_url ?>','<? echo $private_auth_file ?>');"/></td>
		<br>
		<hr>
		<br>
		<h2>Record a private access key into your session</h2>
		<span><b>Private Key</b> : <input class="input-setval " type="password" name="keysession" id="keysession" style="width:350px" placeholder="8 characters at least" value="<? echo $keysession ?>"></span>
		<input type="submit" class="btn btn-success btn-xs" style="width: 100px;" name="reckey" value="Record key" onclick="document.getElementById('message').style.display='none';"/>
		<br>
		<div id="message" style="display: none;"><p style="color:#68b893">Success: your private access key has been reccorded into session</p><div>
	</form>
</section>
<br>

<?php

do {
	// checks if the 'encrypt' button is pressed
	if( isset($_POST['encrypt']) && strlen($_POST['key'])>7 ) {
		exec("/usr/bin/htpasswd -nbs - ".$_POST['key']." | cut -d':' -f2", $key1, $retval);
		if ($reval==0) {
			file_put_contents($crypt_url, $key1[0]);
			echo "<script>document.getElementById('download').style.display='block';</script>";
		}
		break;
	}

	// checks if the 'reckey' button is pressed
	if( isset($_POST['reckey']) && strlen($_POST['keysession'])>7 ) {
		exec("/usr/bin/htpasswd -nbs - ".$_POST['keysession']." | cut -d':' -f2", $key2, $retval);
		if ($reval==0) {
			#print_r($key2[0]); echo "<br>";
			$_SESSION['privatekey'] = $key2[0];
			echo "<script>document.getElementById('message').style.display='block';</script>";
		}
		break;
	}

} while(0);

?>

<br>
