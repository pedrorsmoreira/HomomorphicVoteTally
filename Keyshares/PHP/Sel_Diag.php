<html>
 <body>
 <form action="InsertDiag.php" method="post">
 <h3>Select a new diagnosis code:</h3>
 <p>
 <select name="ID">
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

$VAT_doctor = $_REQUEST['doctor'];
$date_timestamp = $_REQUEST['date'];

$sql = "SELECT ID FROM diagnostic_code
WHERE ID NOT IN(
  SELECT ID FROM consultation_diagnostic
  WHERE date_timestamp = '$date_timestamp'
  AND VAT_doctor = $VAT_doctor) ORDER BY ID";
$result = $connection->query($sql);
if ($result == FALSE)
{
$info = $connection->errorInfo();
echo("<p>Error: {$info[2]}</p>");
exit();
}
foreach($result as $row)
{
$ID = $row['ID'];
echo("<option value=\"$ID\">$ID</option>");
}

 $connection = null;
?>
 </select>
 </p>
 <p><input type="hidden" name="VAT_doctor" value="<?=$_REQUEST['doctor']?>"/></p>
 <p><input type="hidden" name="date_timestamp" value="<?=$_REQUEST['date']?>"/></p>
 <p><input type="hidden" name="VAT_client" value="<?=$_REQUEST['client']?>"/></p>
 <p><input type="submit" value="Submit"/></p>
 </form>
 </body>
</html>
