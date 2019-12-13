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
$connection->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION); // why is this one needed?

$VAT = $_REQUEST['VAT'];
//$name = "%".$_REQUEST['name']."%";
$name = "%{$_REQUEST['name']}%";
$street = "%".$_REQUEST['street']."%";
$city = "%".$_REQUEST['city']."%";
$zip = "%".$_REQUEST['zip']."%";

$query = "SELECT * FROM client
WHERE name like :name
AND street like :street
AND city like :city
AND zip like :zip ";

if (!empty($VAT)){ //Inseriu VAT
  $query .= "AND VAT = :VAT";
  $param = array('name' => $name, 'street' => $street, 'city' => $city, 'zip' => $zip, 'VAT' => $VAT);
}
else { //Nao inseriu VAT
  $param = array('name' => $name, 'street' => $street, 'city' => $city, 'zip' => $zip);
}
$sql = $connection->prepare($query);
$sql->execute($param);

$nrows = $sql->rowCount();
if ($nrows!=0){
  echo ("<h2>Results for client search:</h2>");
  echo("<table border=\"1\">");
  echo("<tr><td>VAT</td><td>Name</td><td>Birth Date</td>
  <td>Street</td><td>City</td><td>Zip</td>
  <td>Gender</td><td>Age</td></tr>");

  foreach($sql as $row)
  {
    echo("<tr><td>");
    echo($row['VAT']);
    echo("</td><td>");
    echo($row['name']);
    echo("</td><td>");
    echo($row['birth_date']);
    echo("</td><td>");
    echo($row['street']);
    echo("</td><td>");
    echo($row['city']);
    echo("</td><td>");
    echo($row['zip']);
    echo("</td><td>");
    echo($row['gender']);
    echo("</td><td>");
    echo($row['age']);
    echo("</td><td><a href=\"NewAppointment.php?VAT=");
    echo($row['VAT']);
    echo("\">Add Apointment</a></td>\n");
    echo("</td><td><a href=\"AppointmentList.php?VAT=");
    echo($row['VAT']);
    echo("\">View Appointments</a></td>\n");
    echo("</tr>");
  }
  echo("</table>");
}
else{ //Nothing returned
  echo("<h2>No clients were found!</h2>");
}
echo ("<p><a href=\"NewClient.php");
echo("\">Add New Client</a></p>\n");
echo ("<p><a href=\"Search_client.php");
echo("\">Return to Client Search</a></p>\n");

 $connection = null;
?>
 </body>
</html>
