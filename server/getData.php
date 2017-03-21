<?php
//Подключаемся к базе данных
$host = "localhost";
$user = "u128217053_m";
$pass = "Lfnfvjtqcvthnb";
$database = "u128217053_tr";
$db = mysql_connect($host, $user, $pass);	
mysql_select_db($database, $db);
mysql_query('SET NAMES UTF8');

$query = "SELECT * FROM translate_table WHERE sync=0"; //Извлекаем данные для синхронизации

$sql = mysql_query($query);
$w1;
$w2;
while ($row = mysql_fetch_object($sql))
{
	$w1 = $row->ruWord;
	$w2 = $row->enWord;
	//echo $w1;
	echo "rus:$w1\n";
	echo "eng:$w2\n";
}

//А теперь отмечаем синхронизацию
$query = "UPDATE translate_table SET sync=1 WHERE sync=0";

$sql = mysql_query($query);
?>