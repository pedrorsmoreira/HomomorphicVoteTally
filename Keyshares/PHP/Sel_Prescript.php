<html>
 <body>
 <form action="InsertPrescript.php" method="post">

<?php
$host = "db.ist.utl.pt";
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
      //Check if there is a diagnosis, as prescription is associated to one of those
$VAT_doctor = $_REQUEST['doctor'];
$date_timestamp = $_REQUEST['date'];
$VAT_client = $_REQUEST['client'];
$sql = "SELECT ID FROM consultation_diagnostic
  WHERE date_timestamp = '$date_timestamp'
  AND VAT_doctor = $VAT_doctor";

$result = $connection->query($sql);
if ($result == FALSE)
{
  $info = $connection->errorInfo();
  echo("<p>Error: {$info[2]}</p>");
  exit();
}

$nrows = $result->rowCount();
if ($nrows == 0)
{
  echo("<h2>Error: No diagnosis for consultation. No procedure can be added!</h2>");
  echo ("<p><a href=\"Sel_Diag.php?doctor=");
  echo($VAT_doctor);
  echo("&date=");
  echo($date_timestamp);
  echo("&client=");
  echo($VAT_client);
  echo("\">Add diagnosis for this consultation</a></p>\n");
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
else{
  echo("<h2>Insert a new Procedure:</h2>\n");
  echo("<h4>Select Associated Diagnosis:</h4>\n");
  echo("<select name=\"ID\" required>");
  foreach ($result as $row) {
    $ID = $row['ID'];
    echo("<option value=\"$ID\">$ID</option>\n");
  }
  echo("</select>\n");
}
echo("<h4>Select medication</h4>\n");
echo("<select name=\"name_lab\" required>\n");
$sql = "SELECT name, lab FROM medication
WHERE name NOT IN(
  SELECT name FROM prescription
  WHERE date_timestamp = '$date_timestamp'
  AND VAT_doctor = $VAT_doctor) ORDER BY name";
$result = $connection->query($sql);
if ($result == FALSE)
{
$info = $connection->errorInfo();
echo("<p>Error: {$info[2]}</p>");
exit();
}
foreach($result as $row)
{
  $name = $row['name'];
  $lab = $row['lab'];
  echo("<option value=\"$name-$lab\">$name - $lab</option>");
}
  echo("</select>");
 $connection = null;
?>
 <p><input type="hidden" name="VAT_doctor" value="<?=$_REQUEST['doctor']?>"/></p>
 <p><input type="hidden" name="date_timestamp" value="<?=$_REQUEST['date']?>"/></p>
 <p><input type="hidden" name="VAT_client" value="<?=$_REQUEST['client']?>"/></p>
 <h4>Dosage:<input type="text" name ="dosage" required/></h4>
 <h4>Description<input type="text" name ="description" required/></h4>
 <p><input type="submit" value="Submit"/></p>
 </form>
 </body>
</html>
