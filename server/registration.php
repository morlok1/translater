<?php
include('db.inc');


header('Origin: http://morlok1.esy.es');
header('HTTP/1.1 200 OK');
header('Content-Type:text/html; charset=UTF-8');
header('Access-Control-Allow-Origin: *');

$state = 0;

$nick = $_POST["nick"]; //Получаем переданный ник

//Проверка на уникальность
$query  = "SELECT nick FROM nick_table";
$sql = mysql_query($query);

while ($row = mysql_fetch_object($sql))
{
	$old_nick = $row -> nick;
	if ($nick == $old_nick)
		$state++;
}

if ($state != 0 || $nick == "") //Ник повторился
	echo "answer:1";
else
{
	$query = "INSERT INTO nick_table (nick) VALUES ('$nick')";
	$dummy = mysql_query($query);
	if ($dummy)//Добавление произошло
		echo "answer:0";
	else //Добавление не произошло
		echo "answer:2";
}
?>