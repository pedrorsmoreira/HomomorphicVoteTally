<html>
 <body>
<?php
$host = "db.tecnico.ulisboa.pt";
$user = "ist181067";
$pass = "tquv2884";
$dsn = "mysql:host=$host;dbname=$user";
try
{
$connection = new PDO($dsn, $user, $pass);
}
catch(PDOException $exception)
{
echo("<p>Error: ");
echo($exception->getMessage());
echo("</p>");
exit();
}
$connection->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
$connection->setAttribute(PDO::ATTR_EMULATE_PREPARES, false);

$VAT_doctor = $_REQUEST['VAT_doctor'];
$date_timestamp = $_REQUEST['date_timestamp'];
$VAT_client = $_REQUEST['VAT_client'];
$SOAP_S = $_REQUEST['SOAP_S'];
$SOAP_O = $_REQUEST['SOAP_O'];
$SOAP_A = $_REQUEST['SOAP_A'];
$SOAP_P = $_REQUEST['SOAP_P'];
$VAT_nurse = $_REQUEST['VAT_nurse'];

//PART 1 - Insert consultation
$query = "INSERT INTO consultation VALUES (:VAT_doctor, :date_timestamp, :SOAP_S, :SOAP_O, :SOAP_A, :SOAP_P)";
$param = array('VAT_doctor' => $VAT_doctor, 'date_timestamp' => $date_timestamp, 'SOAP_S' => $SOAP_S, 'SOAP_O' => $SOAP_O, 'SOAP_A' => $SOAP_A, 'SOAP_P' => $SOAP_P);

try{
  $sql = $connection->prepare($query);
  $result = $sql->execute(array_values($param));
}
catch(PDOException $exception)
{
  echo("<p>Error: ");
  echo($exception->getMessage());
  echo("</p>");
  echo ("<p><a href=\"Search_client.php");
  echo("\">Return to Client Search</a></p>\n");
  echo("<p><a href=\"AddConsultation.php?date=");
  echo($date_timestamp);
  echo("&doctor=");
  echo($VAT_doctor);
  echo("&client=");
  echo($VAT_client);
  echo("\">Insert Consultation Data Again</a></p>\n");
  echo ("<p><a href=\"AppointmentList.php?VAT=");
  echo($VAT_client);
  echo("\">Return to Client's Appointments</a></p>\n");
  exit();
}
echo ("<p>$query</p>");
echo("<p>Where :VAT_doctor = $VAT_doctor;</p>");
echo("<p>Where :date_timestamp = $date_timestamp;</p>");
echo("<p>Where :SOAP_S = $SOAP_S;</p>");
echo("<p>Where :SOAP_O = $SOAP_O;</p>");
echo("<p>Where :SOAP_A = $SOAP_A;</p>");
echo("<p>Where :SOAP_P = $SOAP_P;</p>");

$nrows = $sql->rowCount();
echo("<p>Rows inserted: $nrows</p>");

//PART 2 - Insert Nurse for Consultation
$sql = "INSERT INTO consultation_assistant VALUES ($VAT_doctor, '$date_timestamp', $VAT_nurse)";

$result = $connection->query($sql);
$nrows = $result->rowCount();

echo("<p>$sql</p>");
echo("<p>Affected rows: $nrows</p>");

echo ("<p><a href=\"Search_client.php");
echo("\">Return to Client Search</a></p>\n");
echo ("<p><a href=\"AppointmentList.php?VAT=");
echo($VAT_client);
echo("\">Return to Client's Appointments</a></p>\n");

 $connection = null;
?>
 </body>
</html>
