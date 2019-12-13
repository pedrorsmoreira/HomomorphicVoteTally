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

//QUERY 1 - diagnosis
$sql = "SELECT cd.ID, description
FROM consultation_diagnostic AS cd, diagnostic_code AS d
WHERE cd.ID = d.ID
AND cd.VAT_doctor = $VAT_doctor
AND cd.date_timestamp = '$date_timestamp'";

$result = $connection->query($sql);
$nrows = $result->rowCount();

if($nrows == 0){
  echo("<h3> This consultation has no associated diagnostics! </h3>");
}
else {
  echo("<h2>Associated diagnosis for this consultation</h2>");
  echo("<table border=\"1\">");
  echo("<tr><td>ID</td><td>Description</td></tr>");
  foreach ($result as $row){
    echo("<tr><td>");
    echo($row['ID']);
    echo("</td><td>");
    echo($row['description']);
    echo("</td></tr>");
  }
  echo("</table>");
}
echo ("<p><a href=\"Sel_Diag.php?doctor=");
echo($VAT_doctor);
echo("&date=");
echo($date_timestamp);
echo("&client=");
echo($VAT_client);
echo("\">Add diagnosis for this consultation</a></p>\n");

//Query 2: Asssistant Nurses
$sql = "SELECT VAT_nurse, name
FROM consultation_assistant, employee
WHERE VAT_doctor = $VAT_doctor
AND date_timestamp = '$date_timestamp'
AND VAT_nurse = VAT";

$result = $connection->query($sql);
$nrows = $result->rowCount();

if($nrows == 0){
  echo("<h3>ERROR: This consultation has no associated Nurses! </h3>");
}
else {
  echo("<h2>Assistants for this consultation</h2>");
  echo("<table border=\"1\">");
  echo("<tr><td>Nurse's VAT</td><td>Name</td></tr>");
  foreach ($result as $row){
    echo("<tr><td>");
    echo($row['VAT_nurse']);
    echo("</td><td>");
    echo($row['name']);
    echo("</td></tr>");
  }
  echo("</table>");
}
echo ("<p><a href=\"Sel_Nurses.php?doctor=");
echo($VAT_doctor);
echo("&date=");
echo("$date_timestamp");
echo("&client=");
echo($VAT_client);
echo("\">Add assistant Nurses for this consultation</a></p>\n");


//QUERY 3 - Asssociated prescriptions
$sql = "SELECT name, lab, ID, dosage, description
FROM prescription
WHERE VAT_doctor = $VAT_doctor
AND date_timestamp = '$date_timestamp'";

$result = $connection->query($sql);
$nrows = $result->rowCount();

if($nrows == 0){
  echo("<h3>This consultation has no associated prescriptions! </h3>");
}
else {
  echo("<h2>Prescriptions for this consultation</h2>");
  echo("<table border=\"1\">");
  echo("<tr><td>Name</td><td>Lab</td>");
  echo("<td>Diagnosis ID</td><td>Dosage</td>");
  echo ("<td>Description</td></tr>");
  foreach ($result as $row){
    echo("<tr><td>");
    echo($row['name']);
    echo("</td><td>");
    echo($row['lab']);
    echo("</td><td>");
    echo($row['ID']);
    echo("</td><td>");
    echo($row['dosage']);
    echo("</td><td>");
    echo($row['description']);
    echo("</td></tr>");
  }
  echo("</table>");
}
echo ("<p><a href=\"Sel_Prescript.php?doctor=");
echo($VAT_doctor);
echo("&date=");
echo("$date_timestamp");
echo("&client=");
echo($VAT_client);
echo("\">Add Prescriptions in this consultation</a></p>\n");

//QUERY 4 - Asssociated procedures
$sql = "SELECT name, description
FROM procedure_in_consultation
WHERE VAT_doctor = $VAT_doctor
AND date_timestamp = '$date_timestamp'";

$result = $connection->query($sql);
$nrows = $result->rowCount(); 

if(empty($nrows)){
  echo("<h3>This consultation has no associated procedures! </h3>");
}
else {
  echo("<h2>Procedures for this consultation</h2>");
  echo("<table border=\"1\">");
  echo("<tr><td>Name</td>");
  echo ("<td>Description</td></tr>");
  foreach ($result as $row){
    echo("<tr><td>");
    echo($row['name']);
    echo("</td><td>");
    echo($row['description']);
    echo("</td></tr>");
  }
  echo("</table>");
}

echo ("<p><a href=\"AddDentalCharting.php?doctor=");
echo($VAT_doctor);
echo("&date=");
echo($date_timestamp);
echo("\">Add Dental Charting Procedure for this consultation</a></p>\n"); 

echo ("<p><a href=\"AppointmentList.php?VAT=");
echo($VAT_client);
echo("\">Return to Client's Appointments</a></p>\n");
echo ("<p><a href=\"Search_client.php");
echo("\">Return to Client Search</a></p>\n");

$connection = null;
?>
 </body>
</html>
