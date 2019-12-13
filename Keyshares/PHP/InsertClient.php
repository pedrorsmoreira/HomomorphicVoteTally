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
$connection->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
$connection->setAttribute(PDO::ATTR_EMULATE_PREPARES, false);

$VAT = $_REQUEST['VAT'];
$name = $_REQUEST['name'];
$street = $_REQUEST['street'];
$city = $_REQUEST['city'];
$zip = $_REQUEST['zip'];
$birth_date = $_REQUEST['birth_date'];
$gender = $_REQUEST['gender'];
$phone_number = $_REQUEST['phone_number'];
$age = 1;

$query = "INSERT INTO client VALUES (:VAT, :name, :birth_date, :street, :city, :zip, :gender, :age)";
$param = array('VAT' => $VAT, 'name' => $name, 'birth_date' => $birth_date, 'street' => $street, 'city' => $city, 'zip' => $zip, 'gender' => $gender, 'age' => $age);

try{
  $sql = $connection->prepare($query);
  $result = $sql->execute(array_values($param));
}
catch(PDOException $exception)
{
  echo("<p>Error: ");
  echo($exception->getMessage());
  echo("</p>");
  echo ("<p><a href=\"Search_client.php");
  echo("\">Return to Client Search</a></p>\n");
  echo ("<p><a href=\"NewClient.php");
  echo("\">Add new Client</a></p>\n");
  exit();
}
echo ("<p>$query</p>");
echo("<p>Where :VAT = $VAT;</p>");
echo("<p>Where :name = $name;</p>");
echo("<p>Where :birth_date = $birth_date;</p>");
echo("<p>Where :street = $street;</p>");
echo("<p>Where :city = $city;</p>");
echo("<p>Where :zip = $zip;</p>");
echo("<p>Where :gender = $gender;</p>");
echo("<p>Where :age = $age</p>");

$nrows = $sql->rowCount();
echo("<p>Rows inserted: $nrows</p>");

if(!empty($phone_number)){
  $sql = "INSERT INTO phone_number_client VALUES ($VAT, $phone_number)";
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
}
echo("<p><a href=\"Sel_phone_number.php?client=");
echo($VAT);
echo("\">Add Phone Number for Client</a></p>\n");
echo ("<p><a href=\"Search_client.php");
echo("\">Return to Client Search</a></p>\n");

 $connection = null;
?>
 </body>
</html>
