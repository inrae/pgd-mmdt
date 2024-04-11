<?php

use PHPMailer\PHPMailer\PHPMailer;
use PHPMailer\PHPMailer\Exception;

/**
 * sendMailObj : Send an email to data managers
 * @return the email status
 */
function sendMailObj ($Subject, $Body, $ToAddr, $BCC=0) {
	global $sendMail;
	require '/usr/share/php/vendor/autoload.php';

	if ( ! strlen($ToAddr) ) $ToAddr=$sendMail['UserEmail'];
	$mail = new PHPMailer;
	$mail->SMTPDebug = 1;
	$mail->SMTPAuth = true;
	$mail->IsSMTP();
	$mail->Host  = $sendMail['smtpHost'];
	$mail->SMTPSecure = $sendMail['smtpSecure'];
	$mail->Port = $sendMail['smtpPort'];
	$mail->Username = $sendMail['CheckEmail'];
	$mail->Password = $sendMail['CheckPass'];
	$mail->setFrom($mail->Username, $sendMail['CheckName']);
	$mail->AddAddress($ToAddr);
	if( $BCC ) $mail->AddBCC($sendMail['UserEmail']);

	$mail->Subject = $Subject;
	$mail->Body = $Body;

	$error_return = "";
	if(!$mail->send()) {
		$error_return =  $mail->ErrorInfo;
	}
	return ( $error_return );
}

 
// Send an email to the admin/moderator
if (! is_null($sendMail)) {
	$Subject = "[Maggot] New Dataset";
	$Date = date_default_timezone_set('UTC');
	$Body = <<<EOT
Coucou ! This is a test carried out on $date
EOT;
	$error_return = sendMailObj ($Subject, $Body, '', 0);
	echo "<script>console.log('sendMail : ".$error_return."')</script>";
}

?>
