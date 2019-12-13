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
$namelab = $_REQUEST['name_lab'];
$dosage = $_REQUEST['dosage'];
$description = $_REQUEST['description'];
$ID = $_REQUEST['ID'];
list($name, $lab) = explode("-", "$namelab", 2);

$query = "INSERT INTO prescription VALUES (:name, :lab, :VAT_doctor, :date_timestamp, :ID, :dosage, :description)";
$param = array('name' => $name, 'lab' => $lab, 'VAT_doctor' => $VAT_doctor, 'date_timestamp' => $date_timestamp, 'ID' => $ID, 'dosage' => $dosage, 'description' => $description);

try {
  $sql = $connection->prepare($query);
  $result = $sql->execute(array_values($param));
}
catch (PDOException $e) {
  echo("<p>Error: ");
  echo($e->getMessage());
  echo("</p>");
  echo ("<p><a href=\"Sel_Prescript.php?doctor=");
  echo($VAT_doctor);
  echo("&date=");
  echo("$date_timestamp");
  echo("&client=");
  echo($VAT_client);
  echo("\">Select another Prescription for this consultation</a></p>\n");
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
  exit();
}

echo ("<p>$query</p>");
echo("<p>Where :name = $name;</p>");
echo("<p>Where :lab = $lab;</p>");
echo("<p>Where :VAT_doctor = $VAT_doctor;</p>");
echo("<p>Where :date_timestamp = $date_timestamp;</p>");
echo("<p>Where :ID = $ID;</p>");
echo("<p>Where :dosage = $dosage;</p>");
echo("<p>Where :description = $description;</p>");
$nrows = $sql->rowCount();
echo("<p>Rows inserted: $nrows</p>");

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
