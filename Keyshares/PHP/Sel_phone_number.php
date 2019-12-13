<html>
 <body>
 <form action="InsertPhoneNumber.php" method="post">
 <h2>Insert Client Phone Number:</h2>
 <p><input type="number" name="phone_number" min = "910000000" max = "969999999" required/></p>
 <p><input type="hidden" name="VAT_client" value="<?=$_REQUEST['client']?>"/></p>
 <p><input type="submit"/></p>
 </form>
 </body>
</html>
