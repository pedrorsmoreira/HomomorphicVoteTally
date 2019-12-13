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

$VAT_nurse = $_REQUEST['nurse'];
$VAT_doctor = $_REQUEST['doctor'];
$date_timestamp = $_REQUEST['date'];
$VAT_client = $_REQUEST['client'];

$sql = "INSERT INTO consultation_assistant VALUES ($VAT_doctor, '$date_timestamp', $VAT_nurse)";
$result = $connection->query($sql);

if ($result == FALSE)
{
  $info = $connection->errorInfo();
  echo("<p>Error: {$info[2]}</p>");
  echo ("<p><a href=\"Sel_Nurses.php?doctor=");
  echo($VAT_doctor);
  echo("&date=");
  echo("$date_timestamp");
  echo("&client=");
  echo($VAT_client);
  echo("\">Select another nurse for this consultation</a></p>\n");
}
else{
  echo ("<p>$sql</p>");
  $nrows = $result->rowCount();
  echo("<p>Rows inserted: $nrows</p>");
}
echo("<p><a href=\"ConsultInfo.php?date=");
echo($date_timestamp);
echo("&doctor=");
echo($VAT_doctor);
echo("&client=");
echo($VAT_client);
echo("\">Return to Consultation</a></td>\n");
echo ("<p><a href=\"AppointmentList.php?VAT=");
echo($VAT_client);
echo("\">Return to Client's Appointments</a></p>\n");
echo ("<p><a href=\"Search_client.php");
echo("\">Return to Client Search</a></p>\n");
$connection = null;
?>
</body>
</html>
