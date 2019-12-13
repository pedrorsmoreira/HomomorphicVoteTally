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
$VAT_client = $_REQUEST['client'];


$sql = "SELECT nurse.VAT AS VAT, name FROM nurse, employee
WHERE nurse.VAT NOT IN(
SELECT VAT_nurse FROM consultation_assistant
WHERE DATE_FORMAT(date_timestamp, '%Y-%m-%d %H') = DATE_FORMAT('$date_timestamp', '%Y-%m-%d %H')
AND VAT_doctor = $VAT_doctor)
AND nurse.VAT = employee.VAT";

$result = $connection->query($sql);
$nrows = $result->rowCount();
if ($nrows!=0)
{
  echo("<h3>Nurses available for the given dates:</h3>");
  echo("<table border=\"0\" cellspacing=\"5\">\n");
  echo("<tr><td>Nurse's VAT</td><td>Nurse's name</td></tr>");
  foreach($result as $row)
  {
    echo("<tr><td>");
    echo($row['VAT']);
    echo ("</td><td>");
    echo($row['name']);
    echo("</td><td><a href=\"InsertAssistant.php?doctor=");
    echo($VAT_doctor);
    echo("&date=");
    echo($date_timestamp);
    echo("&nurse=");
    echo($row['VAT']);
    echo("&client=");
    echo($VAT_client);
    echo("\">Select assistant Nurse</a></td>\n");
    echo("</tr>");
  }
  echo("</table>");
}
else{ //Nothing returned
  echo("<h3>No Nurses available for this consultation! The consultation will be deleted!!!</h3>");
}

 $connection = null;
?>
 </body>
</html>
