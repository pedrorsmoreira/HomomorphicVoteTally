<html>
 <body>
 <h3>Insert Measurements:</h3>
 <form action="InsertToothMeasurements.php" method="post">
 <p><input type="hidden" name="VAT" value="<?=$_REQUEST['VAT']?>"/></p>
 <p><input type="hidden" name="date" value="<?=$_REQUEST['date']?>"/></p>
 <p><input type="hidden" name="quadrant" value="<?=$_REQUEST['quadrant']?>"/></p>
 <p><input type="hidden" name="number_teeth" value="<?=$_REQUEST['number_teeth']?>"/></p>
 <h4>Description: <input type="text" name="description" maxlength="255" required></h4>
 <h4>Measure: <input type="number" name="measure" required></h4>
 <p><input type="submit" value="Submit"/></p>
 </form>
 </body>
</html>
