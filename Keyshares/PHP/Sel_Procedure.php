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

$VAT_doctor = $_REQUEST['doctor'];
$date_timestamp = $_REQUEST['date'];

$name = "dental charting";

$sql = "SELECT * FROM teeth";
$result = $connection->query($sql);
$nrows = $result->rowCount();
if ($nrows!=0)
{
  echo("<h3>Select Tooth to add measurements:</h3>");
  echo("<table border=\"0\" cellspacing=\"8\">\n");
  echo("<tr><td>Quadrant</td><td>Number</td><td>Name</td></td></tr>");
  foreach($result as $row)
  {
    echo("<tr><td>");
    echo($row['quadrant']);
    echo ("</td><td>");
    echo($row['number_teeth']);
    echo("</td><td>");
    echo($row['name']);
    echo("</td><td><a href=\"ToothMeasurements.php?VAT=");
    echo($VAT_doctor);
    echo("&date=");
    echo($date_timestamp);
    echo("&name=");
    echo($name);
    echo("&quadrant=");
    echo($row['quadrant']);
    echo("&number_teeth=");
    echo($row['number_teeth']);
    echo("\">Insert measurements</a></td>\n");
    echo("</tr>");
  }
  echo("</table>");
}
else{ //Nothing returned
  echo("<h3>ERROR: No teeth data</h3>");
}

 $connection = null;
?>
 </body>
</html>