<?php
$host = "localhost";
$user = "u128217053_m";
$pass = "Lfnfvjtqcvthnb";
$database = "u128217053_tr";
$db = mysql_connect($host, $user, $pass);	
mysql_select_db($database, $db);
mysql_query('SET NAMES UTF8');

$enWord_g = $_POST['en'];
$ruWord_g = $_POST['ru'];

$query = "INSERT INTO example_table (ex) VALUES (1)";

mysql_query($query);

$query = "INSERT INTO translate_table (enWord, ruWord) VALUES ('$enWord_g', '$ruWord_g')";

if (mysql_query($query))
	echo "O`kay...";
else
	echo "Fuck...";

header('Access-Control-Allow-Origin: http://morlok1.esy.es');
header('Content-type: text/plain');
return $enWord;

/*
$enWord_g = $_GET['en'];
$ruWord_g = $_GET['ru'];


*/
?>

