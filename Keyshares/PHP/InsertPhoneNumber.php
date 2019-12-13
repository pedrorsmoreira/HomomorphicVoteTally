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

$phone_number = $_REQUEST['phone_number'];
$VAT_client = $_REQUEST['VAT_client'];

$sql = "INSERT INTO phone_number_client VALUES ($VAT_client, $phone_number)";
$result = $connection->query($sql);
if ($result == FALSE)
{
  $info = $connection->errorInfo();
  echo("<p>Error: {$info[2]}</p>");
}
else{
  echo("<p>$sql</p>");
  $nrows=$result->rowCount();
  echo("<p> Rows affected: $nrows rows</p>");
}

echo("<p><a href=\"Sel_phone_number.php?client=");
echo($VAT_client);
echo("\">Add Another Phone Number for Client</a></p>\n");
echo("</td><td><a href=\"AppointmentList.php?VAT=");
echo($VAT_client);
echo("\">View Client Records</a></td>\n");
echo ("<p><a href=\"Search_client.php");
echo("\">Return to Client Search</a></p>\n");

 $connection = null;
?>
 </body>
</html>
