<html>
 <body>
 <form action="InsertClient.php" method="post">
 <h2>Insert Client Data</h2>
 <h4>VAT: <input type="number" name="VAT" min="100000000" max="999999999" required/></h4>
 <h4>Name: <input type="text" name="name" maxlength="50" required/></h4>
 <h4>Birth Date: <input type="date" name="birth_date" min="1900-01-01" max = <?=date("Y-m-d")?> required/></h4>
 <h4>Street: <input type="text" name="street" maxlength="40"/ required></h4>
 <h4>City: <input type="text" name="city" maxlength="30"/ required></h4>
 <h4>ZIP: <input type="text" name="zip" maxlength="15" required/></h4>
 <h4>Gender:</h4>
 <p>Male <input type="radio" name="gender" value="M" required/></p>
 <p>Female <input type="radio" name="gender" value="F"/></p>
 <p>Other <input type="radio" name="gender" value="O"/></p>
 <h4>Phone number (optional) <input type="number" name="phone_number" min = "910000000" max = "969999999"/></h4>
 <p><input type="submit"/></p>
 </form>
 </body>
</html>
