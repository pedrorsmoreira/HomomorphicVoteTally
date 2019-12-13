<html>
 <body>
 <h2>Insert Consultation Details:</h2>
 <form action="InsertConsultation.php" method="post">
 <p><input type="hidden" name="VAT_doctor" value="<?=$_REQUEST['doctor']?>"/></p>
 <p><input type="hidden" name="date_timestamp" value="<?=$_REQUEST['date']?>"/></p>
 <p><input type="hidden" name="VAT_client" value="<?=$_REQUEST['client']?>"/></p>
 <h4>Subjective Observation:<input type="text" name="SOAP_S" maxlength="255" required</h4>
 <h4>Objective Observation:<input type="text" name="SOAP_O" maxlength="255" required </h4>
 <h4>Asssessment:<input type="text" name="SOAP_A" maxlength="255" required</h4>
 <h4>Plan:<input type="text" name="SOAP_P" maxlength="255" required</h4>
 <h4>Select Assistant Nurse: </h4>
 <select name="VAT_nurse" required>
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
 $date_timestamp = $_REQUEST['date'];
 $VAT_doctor = $_REQUEST['doctor'];
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
   foreach($result as $row)
   {
     $VAT_nurse = $row['VAT'];
     $name = $row['name'];
     echo("<option value =\"$VAT_nurse\">$VAT_nurse - $name</option>");
   }
 }
 else{ //Nothing returned
   echo("<h3>No Nurses available for this consultation! The consultation will not be inserted!!!</h3>");
   echo ("<p><a href=\"AppointmentList.php?VAT=");
   echo($VAT_client);
   echo("\">Return to Client's Appointments</a></p>\n");
   echo ("<p><a href=\"Search_client.php");
   echo("\">Return to Client Search</a></p>\n");
   exit();
 }
  $connection = null;
  ?>
</select>
 <p>Note: Further nurses must be added when viewing consultation details.</p>
 <p><input type="submit" value="Submit"/></p>
 </form>
 </body>
</html>
