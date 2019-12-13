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

$VAT = $_REQUEST['doctor'];
$date_timestamp = $_REQUEST['date'];

$name = "dental charting";
$description = "aaa";

$connection->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
$connection->setAttribute(PDO::ATTR_EMULATE_PREPARES, false);

//Insert Dental Charting in procedure_in_consultation
$query = "INSERT INTO procedure_in_consultation VALUES (:name, :VAT_doctor, :date_timestamp, :description)";
$param = array('name' => $name, 'VAT_doctor' => $VAT, 'date_timestamp' => $date_timestamp, 'description' => $description);

try {
  $sql = $connection->prepare($query);
  $result = $sql->execute(array_values($param));
}
catch (PDOException $e) {
  echo("<p>Error: ");
  echo($e->getMessage());
  echo("</p>");
  echo ("<p><a href=\"Sel_Procedure.php?doctor=");
  echo($VAT);
  echo("&date=");
  echo($date_timestamp);
  echo("\">Return to Select Tooth</a></p>\n"); 
  echo("<p><a href=\"ToothMeasurements.php?VAT=");
  echo($VAT);
  echo("&date=");
  echo($date_timestamp);
  echo("&name=");
  echo($name);
  echo("&quadrant=");
  echo($quadrant);
  echo("&number_teeth=");
  echo($number_teeth);
  echo("\">Return to Insert Measurements</a></n>\n");
  exit();
}
 
echo ("<p>$query</p>");
echo("<p>Where :name = $name;</p>");
echo("<p>Where :VAT_doctor = $VAT;</p>");
echo("<p>Where :date_timestamp = $date_timestamp;</p>");
echo("<p>Where :description = $description;</p>");
$nrows = $sql->rowCount();
echo("<p>Rows inserted: $nrows</p>"); 

echo ("<p><a href=\"Sel_Procedure.php?doctor=");
echo($VAT);
echo("&date=");
echo($date_timestamp);
echo("\">Select tooth to add measurements</a></p>\n"); 

$connection = null;
?>
 </body>
</html>