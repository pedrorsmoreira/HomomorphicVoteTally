<html>
 <body>
 <h3>Insert description for the appointment:</h3>
 <form action="ScheduleAppointment.php" method="post">
 <p><input type="hidden" name="VAT_client" value="<?=$_REQUEST['VAT_client']?>"/></p>
 <p><input type="hidden" name="VAT_doctor" value="<?=$_REQUEST['VAT_doctor']?>"/></p>
 <p><input type="hidden" name="date_timestamp" value="<?=$_REQUEST['date_timestamp']?>"/></p>
 <p><input type="text" name="description" maxlength="255" required></p>
 <p><input type="submit" value="Submit"/></p>
 </form>
 </body>
</html>
