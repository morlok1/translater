<?php
include('db.inc');


header('Origin: http://morlok1.esy.es');
header('HTTP/1.1 200 OK');
header('Content-Type:text/html; charset=UTF-8');
header('Access-Control-Allow-Origin: *');

$state = 0;

$nick = $_POST["nick"]; //Получаем переданный ник

$query  = "SELECT nick FROM nick_table";
$sql = mysql_query($query);

while ($row = mysql_fetch_object($sql))
{
	$old_nick = $row -> nick;
	if ($nick == $old_nick)
		$state++;
}

if ($state == 0) //Ник не повторялся
	echo "answer:0";
else 			 //Такой ник уже есть
	echo "answer:1";

?>