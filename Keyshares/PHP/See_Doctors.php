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

$VAT_client = $_REQUEST['VAT_client'];
$date = $_REQUEST['date'];
$time = $_REQUEST['timestamp'];
$date_timestamp = date('Y-m-d H', strtotime("$date $time"));

$sql = "SELECT doctor.VAT AS VAT, name FROM doctor, employee
WHERE doctor.VAT NOT IN(
SELECT VAT_doctor FROM appointment
WHERE DATE_FORMAT(date_timestamp, '%Y-%m-%d %H') = DATE_FORMAT('$date_timestamp', '%Y-%m-%d %H'))
AND doctor.VAT = employee.VAT";

$result = $connection->query($sql);
$nrows = $result->rowCount();
if ($nrows!=0)
{
  echo("<h3>Doctors available for the given dates:</h3>");
  echo("<table border=\"0\" cellspacing=\"5\">\n");
  echo("<tr><td>Doctor's VAT</td><td>Doctor's name</td></tr>");
  foreach($result as $row)
  {
    echo("<tr><td>");
    echo($row['VAT']);
    echo ("</td><td>");
    echo($row['name']);
    echo("</td><td><a href=\"Appointment_Description.php?VAT_doctor=");
    echo($row['VAT']);
    echo("&date_timestamp=");
    echo($date_timestamp);
    echo("&VAT_client=");
    echo($VAT_client);
    echo("\">Schedule Apointment</a></td>\n");
    echo("</tr>");
  }
  echo("</table>");
}
else{ //Nothing returned
  echo("<h3>No doctors are available for appointments at the given date!</h3>");
}
echo ("<p><a href=\"Search_client.php");
echo("\">Return to Client Search</a></p>\n");
echo("<p><a href=\"NewAppointment.php?VAT=");
echo($VAT_client);
echo("\">Select another date and time</a></p>");
 $connection = null;
?>
 </body>
</html>
