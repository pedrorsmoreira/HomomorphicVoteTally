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

$VAT_client = $_REQUEST['VAT'];

$sql = "SELECT * FROM appointment
WHERE VAT_client = $VAT_client
AND (VAT_doctor, date_timestamp) NOT IN
(SELECT VAT_doctor, date_timestamp FROM consultation)";

$result = $connection->query($sql);

$nrows = $result->rowCount();
if ($nrows!=0){
  echo("<h2> Appointments without Consultations</h2>");
  echo("<table border=\"1\">");
  echo("<tr><td>Date</td><td>Doctor VAT</td>
  <td>Description</td></tr>");

  foreach($result as $row)
  {
    echo("<tr><td>");
    echo($row['date_timestamp']);
    echo("</td><td>");
    echo($row['VAT_doctor']);
    echo("</td><td>");
    echo($row['description']);
    echo("</td><td><a href=\"AddConsultation.php?date=");
    echo($row['date_timestamp']);
    echo("&doctor=");
    echo($row['VAT_doctor']);
    echo("&client=");
    echo($VAT_client);
    echo("\">Add Consultation Data</a></td>\n");
    echo("</tr>");
  }
  echo("</table>");
}
else{ //Nothing returned
  echo("<h3>No appointments without consultations were found for this client!</h3>");
  echo("</td><td><a href=\"NewAppointment.php?VAT=");
  echo($VAT_client);
  echo("\">Add Apointment</a></td>\n");
}

echo("<p></p><p></p><p></p><p></p>");

//Query 2 - Display consultations:
$sql = "SELECT * FROM appointment NATURAL JOIN consultation
WHERE VAT_client = $VAT_client
ORDER BY date_timestamp";


$result = $connection->query($sql);
$nrows = $result->rowCount();
if ($nrows!=0){
  echo("<h2>Consultations</h2>");
  echo("<table border=\"1\">");
  echo("<tr><td>Date</td><td>Doctor VAT</td>
  <td>Description</td><td>SOAP_S</td><td>SOAP_O</td>
  <td>SOAP_A</td><td>SOAP_P</td></tr>");

  foreach($result as $row)
  {
    echo("<tr><td>");
    echo($row['date_timestamp']);
    echo("</td><td>");
    echo($row['VAT_doctor']);
    echo("</td><td>");
    echo($row['description']);
    echo("</td><td>");
    echo($row['SOAP_S']);
    echo("</td><td>");
    echo($row['SOAP_O']);
    echo("</td><td>");
    echo($row['SOAP_A']);
    echo("</td><td>");
    echo($row['SOAP_P']);
    echo("</td><td><a href=\"ConsultInfo.php?date=");
    echo($row['date_timestamp']);
    echo("&doctor=");
    echo($row['VAT_doctor']);
    echo("&client=");
    echo($row['VAT_client']);
    echo("\">View Further Details</a></td>\n");
    echo("</tr>");
  }
  echo("</table>");
}
else{ //Nothing returned
  echo("<h3>No consultations were found for this client!</h3>");
}

$sql = "SELECT * FROM phone_number_client
WHERE VAT = $VAT_client";

$result = $connection->query($sql);
$nrows = $result->rowCount();
if ($nrows!=0){
  echo("<h2>Client phone numbers:</h2>");
  foreach($result as $row)
  {
    echo("<p>");
    echo($row['phone']);
    echo("</p>");
  }
}
else{
  echo("<h3>Client has no associated phone numbers!</h3>");
}
echo("<p><a href=\"Sel_phone_number.php?client=");
echo($VAT_client);
echo("\">Add Phone Number for Client</a></p>\n");

echo ("<p><a href=\"Search_client.php");
echo("\">Return to Client Search</a></p>\n");

 $connection = null;
?>
 </body>
</html>
