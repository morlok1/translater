//Количество символов
var numOfChar = 0;

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
	
	document.addEventListener('keydown', function()
	{
		if (event.keyCode === 13)
		{	
			numOfChar = 0;
			getTranslate();
		}
		
		// Отменяем действие браузера
		return false;
	});
}

//Собственно, перевод
function getTranslate()
{
	var trText = document.translateForm.translateText.value.trim();
	var langDirect = getDirectTranslate(trText);
	langDirect = (langDirect === 1 ? "ru-en" : (langDirect === 2 ? "en-ru" : langDirect));
	if (trText.length === 0) //Если поле ввода пусто
		document.getElementById('translateResult').innerHTML = "";
	if ((trText.length - numOfChar > 1 || numOfChar - trText.length > 1) && langDirect === "en-ru") 
	{//Если мы не печатаем и направление ввода корректно
		
		numOfChar = trText.length; //Обнуляем это количество
		//Ключ доступа
		var key = "dict.1.1.20170318T103102Z.6bf3bd6d3840a4a3.e5ee46ccab2ac06111418309c4034544d15590f4";
		//Направление перевода
		//var langDirect = "en-ru";
		//Запрос к API Yandex.Translate
		var url = "https://dictionary.yandex.net/api/v1/dicservice/lookup?" 
		+ "key=" + key
		+ "&lang=" + langDirect
		+ "&text=" + trText;

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
			getAnswerToUser(xhr);
		}
	}
	else
		numOfChar = trText.length;
	
	getDirectTranslate(trText);
}	

function getAnswerToUser(xhr)
{
	//Извлекаем XML из ответа сервера
	var xmlReq = xhr.responseXML;
	//Извлечение слова из запроса
	var requestWord = xmlReq.getElementsByTagName("text")[0].innerHTML;
	//Извлечение перевода
	var transalteWord = xmlReq.getElementsByTagName("text")[1].innerHTML;
	//Размещаем перевод на странице
	document.getElementById('translateResult').innerHTML = transalteWord;// + "|" + numOfChar + "-" + trText.length;
			
	//Начинаем отправку данных на сервер
	var saveData = new XMLHttpRequest();
	var urlData = "http://morlok1.esy.es/saveData.php";
	saveData.open('POST',  urlData , false);
	saveData.setRequestHeader("Content-Type","application/x-www-form-urlencoded; charset=utf-8");
	var args = "en=" + requestWord + "&ru=" + transalteWord;
	saveData.send(args);
}

function getDirectTranslate(text)
{
	var rus = /[а-я]/i.test(text);
	var eng = /[a-z]/i.test(text);
	
	if (rus && !eng) //Русское слово
		return 1;
	else if (!rus && eng) //Английское слово
		return 2;
	else
		return 0;
}	