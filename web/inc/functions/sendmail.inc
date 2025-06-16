<?php

# Nom du fichier: sendmail.inc
# Auteur(s): D.JACOB 
# Copyright: (c) INRAE - 2015-2024
#

use PHPMailer\PHPMailer\PHPMailer;
use PHPMailer\PHPMailer\Exception;

/**
 * sendMailObj : Send an email to data managers
 * @return the email status
 */
function sendMailObj ($Subject, $Body, $ToAddr, $BCC=0, $attach=NULL) {
	global $SENDMAIL;
	require '/usr/share/php/vendor/autoload.php';

	if ( ! strlen($ToAddr) ) $ToAddr=$SENDMAIL['UserEmail'];
	$mail = new PHPMailer;
	$mail->SMTPDebug = 0;
	$mail->SMTPAuth = true;
	$mail->IsSMTP();
	$mail->Host  = $SENDMAIL['smtpHost'];
	$mail->SMTPSecure = $SENDMAIL['smtpSecure'];
	$mail->Port = $SENDMAIL['smtpPort'];
	$mail->Username = $SENDMAIL['CheckEmail'];
	$mail->Password = $SENDMAIL['CheckPass'];
	$mail->setFrom($mail->Username, $SENDMAIL['CheckName']);
	if (! preg_match("/,/", $ToAddr)) {
		$mail->AddAddress($ToAddr);
	} else {
		$arr_addr = preg_split("/\s?,\s?/", $ToAddr);
		foreach ($arr_addr as $addr)
		$mail->AddAddress($addr);
	}
	if( $BCC ) $mail->AddBCC($SENDMAIL['UserEmail']);
	if (! is_null($attach) && file_exists($attach)) $mail->addAttachment($attach);

	$mail->Subject = $Subject;
	$mail->Body = $Body;

	$error_return = "";
	if(!$mail->send()) {
		$error_return =  $mail->ErrorInfo;
	}
	return ( $error_return );
}

?>
