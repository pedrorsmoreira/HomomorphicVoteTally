<html>
 <body>
 <h3>Chose date for <?=$_REQUEST['VAT']?>'s appointment</h3>
 <form action="See_Doctors.php" method="post">
 <p><input type="hidden" name="VAT_client"
value="<?=$_REQUEST['VAT']?>"/></p>
 <p>Desired Date: <input type="date" name="date" min="2017-01-01" max="2023-12-31" required/></p>
 <p>Desired Hour: <input type="time" name="timestamp" min = "09:00" max = "17:00" step="3600" required/></p>
 <p><input type="submit" value="Submit"/></p>
 </form>
 </body>
</html>
