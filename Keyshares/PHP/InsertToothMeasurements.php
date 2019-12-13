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

$VAT = $_REQUEST['VAT'];
$date_timestamp = $_REQUEST['date'];
$name = "dental charting";
$quadrant = $_REQUEST['quadrant'];
$number_teeth = $_REQUEST['number_teeth'];
$description = $_REQUEST['description'];
$measure = $_REQUEST['measure'];

$connection->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
$connection->setAttribute(PDO::ATTR_EMULATE_PREPARES, false);

//Now insert Dental Charting info in procedure_charting
$query = "INSERT INTO procedure_charting VALUES (:name, :VAT, :date_timestamp, :quadrant, :number_teeth, :des_, :measure)";
$param = array('name' => $name, 'VAT' => $VAT, 'date_timestamp' => $date_timestamp, 'quadrant' => $quadrant, 'number_teeth' => $number_teeth, 'des_' => $description, 'measure' => $measure);

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
echo("<p>Where :VAT = $VAT;</p>");
echo("<p>Where :date_timestamp = $date_timestamp;</p>");
echo("<p>Where :quadrant = $quadrant;</p>");
echo("<p>Where :number_teeth = $number_teeth;</p>");
echo("<p>Where :des_ = $description;</p>");
echo("<p>Where :measure = $measure;</p>");
$nrows = $sql->rowCount();
echo("<p>Rows inserted: $nrows</p>");

echo("</p>");
echo ("<p><a href=\"Sel_Procedure.php?doctor=");
echo($VAT);
echo("&date=");
echo($date_timestamp);
echo("\">Return to Select Tooth</a></p>\n");

 $connection = null;
?>
 </body>
</html>
