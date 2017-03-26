<?php
//Подключаемся к базе данных
include('db.inc');

$enWord_g = $_POST['en'];
$ruWord_g = $_POST['ru'];
$nick_g = $_POST['nick'];

//Сначала необходимо сделать проверку на существование данной пары перевода
if ($enWord_g != "" && $ruWord_g != "")
{
	$query = "SELECT id FROM translate_table WHERE enWord='$enWord_g' and ruWord='$ruWord_g' and nick='$nick_g'";
	$sql = mysql_query($query);
	$bool = 0;
	while ($row = mysql_fetch_object($sql))
		$bool++;

	//Если пара перевода новая
	if ($bool == 0)
	{
		$query = "INSERT INTO translate_table (enWord, ruWord, nick, sync) VALUES ('$enWord_g', '$ruWord_g', '$nick_g', 0)";
		mysql_query($query);
	}
}

//Заголовки для кроссдоменных запросов, чтобы не ругался js ни у кого
header('Origin: http://morlok1.esy.es');
header('HTTP/1.1 200 OK');
header('Content-Type:text/html; charset=UTF-8');
header('Access-Control-Allow-Origin: *');

//header('Access-Control-Allow-Origin: http://morlok1.esy.es');
//header('Content-type: text/plain');
?>

