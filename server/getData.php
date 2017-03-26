<?php
//Подключаемся к базе данных
include('db.inc');

$nick = $_POST['nick'];

$query = "SELECT * FROM translate_table WHERE sync=0 and nick='$nick'"; //Извлекаем данные для синхронизации

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
$query = "UPDATE translate_table SET sync=1 WHERE sync=0 and nick='$nick'";

$sql = mysql_query($query);
?>