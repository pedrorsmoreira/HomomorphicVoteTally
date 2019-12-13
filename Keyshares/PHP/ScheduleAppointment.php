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
$connection->setAttribute(PDO::ATTR_EMULATE_PREPARES, false);
$connection->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);

$VAT_client = $_REQUEST['VAT_client'];
$VAT_doctor = $_REQUEST['VAT_doctor'];
$date_timestamp = $_REQUEST['date_timestamp'];
$description = $_REQUEST['description'];


$query = "INSERT INTO appointment VALUES ($VAT_doctor, '$date_timestamp', :description, $VAT_client)";

try{
  $sql = $connection->prepare($query);
  $result = $sql->execute(array('description' =>$description));
}
catch(PDOException $exception)
{
  echo("<p>Error: ");
  echo($exception->getMessage());
  echo("</p>");
  echo ("<p><a href=\"Search_client.php");
  echo("\">Return to Client Search</a></p>\n");
  exit();
}
echo ("<p>$query</p>");
echo ("<p>Where :description = $description");
$nrows = $sql->rowCount();
echo("<p>Rows inserted: $nrows</p>");

echo ("<p><a href=\"Search_client.php");
echo("\">Return to Client Search</a></p>\n");
echo ("<p><a href=\"AppointmentList.php?VAT=");
echo($VAT_client);
echo("\">Go to Client's Appointments</a></p>\n");
$connection = null;
?>
</body>
</html>
