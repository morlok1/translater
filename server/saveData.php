<?php
//Подключаемся к базе данных
$host = "localhost";
$user = "u128217053_m";
$pass = "Lfnfvjtqcvthnb";
$database = "u128217053_tr";
$db = mysql_connect($host, $user, $pass);	
mysql_select_db($database, $db);
mysql_query('SET NAMES UTF8');

$enWord_g = $_POST['en'];
$ruWord_g = $_POST['ru'];

//Сначала необходимо сделать проверку на существование данной пары перевода
$query = "SELECT id FROM translate_table WHERE enWord='$enWord_g' and ruWord='$ruWord_g'";
$sql = mysql_query($query);
$bool = 0;
while ($row = mysql_fetch_object($sql))
	$bool++;

//Если пара перевода новая
if ($bool == 0)
{
	$query = "INSERT INTO translate_table (enWord, ruWord, sync) VALUES ('$enWord_g', '$ruWord_g', 0)";
	mysql_query($query);
}

//header('Access-Control-Allow-Origin: http://morlok1.esy.es');
//header('Content-type: text/plain');
?>

