//Ждем загрузки расширения
document.addEventListener('DOMContentLoaded', function () 
{
  getStart();
});

//Закрепляем слушателей событий
function getStart()
{
	//Слушаем текстовое поле на изменение
	document.translateForm.translateText.addEventListener('input', function ()
	{
		getTranslate();
	});
  
	//Слушаем отправку формы, чтобы заглушить это дело
	document.addEventListener('submit', function()
	{
		event.preventDefault();
	});

}

//Собственно, перевод
function getTranslate()
{
	//Ключ доступа
	var key = "dict.1.1.20170318T103102Z.6bf3bd6d3840a4a3.e5ee46ccab2ac06111418309c4034544d15590f4";
	//Направление перевода
	var langDirect = "en-ru";
	//Запрос к API Yandex.Translate
	var url = "https://dictionary.yandex.net/api/v1/dicservice/lookup?" 
	+ "key=" + key
	+ "&lang=" + langDirect
	+ "&text=" + document.translateForm.translateText.value;

	var xhr = new XMLHttpRequest();

	//Конфигурируем запрос
	xhr.open('GET', url, false);
	//Посылаем запрос
	xhr.send();	
	if (xhr.status != 200) 
	{
		document.getElementById('translateResult').innerHTML = "Some thing wrong...";
	} 
	else 
	{
		//Извлекаем XML из ответа сервера
		var xmlReq = xhr.responseXML;
		//Извлечение слова из запроса
		var requestWord = xmlReq.getElementsByTagName("text")[0].innerHTML;
		//Извлечение перевода
		var transalteWord = xmlReq.getElementsByTagName("text")[1].innerHTML;
		//Размещаем перевод на странице
		document.getElementById('translateResult').innerHTML = transalteWord;
		
		//Начинаем отправку данных на сервер
		var saveData = new XMLHttpRequest();
		var urlData = "http://morlok1.esy.es/saveData.php";
		saveData.open('POST',  urlData , false);
		saveData.setRequestHeader("Content-Type","application/x-www-form-urlencoded; charset=utf-8");
		var args = "en=" + requestWord + "&ru=" + transalteWord;
		saveData.send(args);
	}
}			