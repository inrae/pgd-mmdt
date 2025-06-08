<?php

$ID = genKEY();
$crypt_url = 'tmp/'.$ID.'.txt';
$key = isset($_POST['key']) ? $_POST['key'] : '';
$keyc = isset($_POST['keyc']) ? $_POST['keyc'] : '';
$keysession = isset($_POST['keysession']) ? $_POST['keysession'] : '';

$disp1 = 'none';
$disp2 = 'none';
$disp3 = 'none';
$nbcar = 6;

do {
	// checks if the 'encrypt' button is pressed
	$errormsg = '';
	if( isset($_POST['encrypt']) ) {
		if (strlen($key)<$nbcar ) {
			$errormsg = 'Your key has less then '.$nbcar.' characters';
		}
		if (!$errormsg && $keyc !== $key ) { 
			$errormsg = 'The two keys are not identical';
		}
		if ($errormsg) {
			$disp2 = 'block';
			break;
		}
		exec("/usr/bin/htpasswd -nbs - ".$key." | cut -d':' -f2", $key1, $retval);
		if ($retval==0) {
			file_put_contents($crypt_url, $key1[0]);
			$disp1 = 'block';
		}
		break;
	}

	// checks if the 'reckey' button is pressed
	if( isset($_POST['reckey']) && strlen($_POST['keysession'])>=$nbcar ) {
		exec("/usr/bin/htpasswd -nbs - ".$_POST['keysession']." | cut -d':' -f2", $key2, $retval);
		if ($retval==0) {
			#setcookie('privatekey', $key2[0], time() + 24*3600); # see is_accessible() in inc/functions/functions.inc
			$_SESSION['privatekey'] = $key2[0];
			$disp3 = 'block';
		}
		break;
	}

} while(0);

?>

<!DOCTYPE html>
<html lang="en" >

<head>
	<title>Maggot - Encryption</title>
	<?php echo $HEAD ?>
</head>

<body style="zoom: <? echo $ZOOMWP ?>">

<?php echo $header_title ?>
<br><br>

<h1>Private access key management <a href="<? echo $maggot_doc_url.'private-access/' ?>" target="_blank"><i class="bi bi-question-octagon-fill" style="color:grey; font-size: 20px"></i></a></h1>
<br>

<section>
	<br/>
	<form id="formconvert" name="formconvert" action="#" autocomplete="off" method="post" enctype="multipart/form-data">
		<h2>Generate a private access key file</h2>
		<br>
		<div class="form-group2" style="width:250px" id="d1">
		<span class="labeldiv">Private Key</span>
		<fieldset>
			<input class="input-setval " type="password" name="key" id="key" style="width:250px" placeholder="at least <? echo $nbcar ?> characters" value="<? echo $key ?>">
		</fieldset>
		</div>
		&nbsp; &nbsp;
		<div class="form-group2" style="width:250px" id="d1">
		<span class="labeldiv">Confirmation</span>
		<fieldset>
			<input class="input-setval " type="password" name="keyc" id="keyc" style="width:250px" placeholder="at least <? echo $nbcar ?> characters" value="">
		</fieldset>
		</div>
		&nbsp; &nbsp;
		<div class="form-group2" style="width:250px" id="d1">
		<span class="labeldiv"></span>
		<fieldset>
			<input type="submit" class="btn btn-success btn-xs" style="width: 100px;" name="encrypt" value="Encryption" />
		</fieldset>
		</div>
		<br><br>
		<input id="download" type="download" value="Download the Private Access File" class="btn btn-success btn-xs" style="width: 300px; display:<? echo $disp1 ?>;" onclick="downloadFile('<? echo $PATH.$crypt_url ?>','<? echo $private_auth_file ?>');"/>
		<div id="message2" style="display: <? echo $disp2 ?>;"><p style="color:#d92b48">ERROR: <? echo $errormsg ?></p></div>
		<br><br>
		<hr>
		<br><br>
		<h2>Record a private access key into the current session</h2>
		<br>
		<span><b>Private Key</b> : <input class="input-setval " type="password" name="keysession" id="keysession" style="width:350px" placeholder="at least <? echo $nbcar ?> characters" value="<? echo $keysession ?>" onclick="document.getElementById('message').style.display='none';"></span>
		<input type="submit" class="btn btn-success btn-xs" style="width: 100px;" name="reckey" value="Record key" onclick="document.getElementById('message').style.display='none';"/>
		<br><br>
		<div id="message3" style="display: <? echo $disp3 ?>;"><p style="color:#68b893">Success: your private access key has been reccorded</p></div>
	</form>
</section>
<br>

