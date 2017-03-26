//Авторизация или регистрация? 
var state = 0; 
// 0 - авторизация
// 1 - регистрация

//Ждем загрузки расширения
document.addEventListener('DOMContentLoaded', function () 
{
  getStart();
});

function getStart()
{
	var nick = getCookie("nick");
	if (nick === undefined)
	{//Если мы не авторизированы
		document.authForm.authReg.value = 1;
		
		//Слушаем текстовое поле на изменение
		document.authForm.authName.addEventListener('input', function ()
		{
			checkNick();
		});
		
		//Слушаем отправку формы, чтобы заглушить это дело
		document.addEventListener('submit', function()
		{
			event.preventDefault();
		});
		
		
		document.authForm.sub.removeEventListener( "click", exit);
		
		document.authForm.sub.addEventListener('click', submitData);
		
	}
	else
	{
		document.authForm.authName.value = "Авторизованы как " + nick;
		document.authForm.authName.disabled = true;
		document.authForm.authReg.disabled = true;
		document.authForm.sub.value = "Выйти";
		
		document.authForm.sub.addEventListener( "click", exit);
		
		document.authForm.sub.removeEventListener('click', submitData);
	}
}

function checkNick()
{
	if (document.authForm.authReg.value === '0')
	{//Если операция регистрации
		console.log(document.authForm.authReg.value);
		var saveData = new XMLHttpRequest();
		var urlData = "http://morlok1.esy.es/checkNickName.php";
		saveData.open('POST',  urlData , false);
		saveData.setRequestHeader("Content-Type","application/x-www-form-urlencoded; charset=utf-8");
		var args = "nick=" + document.authForm.authName.value;
		
		saveData.send(args);
		var answer = saveData.responseText;
		
		if (answer.indexOf("answer:0") !== -1)
		{//Это новый ник
			document.getElementById("answer").style.color = "green";
			document.getElementById("answer").innerHTML = "Логин свободен";
			document.authForm.sub.disabled = false;
		}
		else
		{//Такой ник уже существует
			document.getElementById("answer").style.color = "red";
			document.getElementById("answer").innerHTML = "Логин занят";	
			document.authForm.sub.disabled = true;
		}
	}
}

function submitData()
{
	if (document.authForm.authReg.value === '0')
	{//Регистрация
		getRegistration();
	}
	else
	{//Авторизация
		getAuthorization();
	}
}

function getRegistration()
{
	var saveData = new XMLHttpRequest();
	var urlData = "http://morlok1.esy.es/registration.php";
	saveData.open('POST',  urlData , false);
	saveData.setRequestHeader("Content-Type","application/x-www-form-urlencoded; charset=utf-8");
	var args = "nick=" + document.authForm.authName.value;
	
	saveData.send(args);
	var answer = saveData.responseText;
	
	
	if (answer.indexOf("answer:0") !== -1)
	{//Регистрация подтверждена
		document.getElementById("answer").style.color = "green";
		document.getElementById("answer").innerHTML = "Регистрация подтверждена";
		document.authForm.authName.value = '';
		document.authForm.authReg.value = 1;
	}
	else if (answer.indexOf("answer:1") !== -1)
	{//Ошибка входных данных
		document.getElementById("answer").style.color = "red";
		document.getElementById("answer").innerHTML = "Введен неверный или пустой логин";
	}
	else
	{//Ошибка работы с бд
		document.getElementById("answer").style.color = "red";
		document.getElementById("answer").innerHTML = "Упс... Что-то пошло не так...";
	}
}

function getAuthorization()
{
	var saveData = new XMLHttpRequest();
	var urlData = "http://morlok1.esy.es/checkNickName.php";
	saveData.open('POST',  urlData , false);
	saveData.setRequestHeader("Content-Type","application/x-www-form-urlencoded; charset=utf-8");
	var args = "nick=" + document.authForm.authName.value;
	
	saveData.send(args);
	var answer = saveData.responseText;
	
	if (answer.indexOf("answer:1") !== -1)
	{//Авторизация подтверждена
		document.getElementById("answer").innerHTML = "";
		//А вот здесь бы нам сделать какую-то запись в storage.local
		setCookie('nick', document.authForm.authName.value, "");
		
		document.authForm.authName.value = "Авторизованы как " + getCookie('nick');
		document.authForm.authName.disabled = true;
		document.authForm.sub.value = "Выйти";
		
		document.authForm.sub.addEventListener( "click", exit);
		
		document.authForm.sub.removeEventListener('click', submitData);
		
	}
	else
	{//Авторизация отклонена
		document.getElementById("answer").style.color = "red";
		document.getElementById("answer").innerHTML = "Введен неверный или пустой логин";
	}
}


function exit()
{//Разавторизируемся обратно
	deleteCookie('nick');
	
	//Выставляем все в изначальное состояние
	document.authForm.authReg.value = 1;
		
	//Слушаем текстовое поле на изменение
	document.authForm.authName.addEventListener('input', function ()
	{
		checkNick();
	});
		
	//Слушаем отправку формы, чтобы заглушить это дело
	document.addEventListener('submit', function()
	{
		event.preventDefault();
	});
	
	document.authForm.sub.removeEventListener( "click", exit);
		
	document.authForm.sub.addEventListener('click', submitData);
	
	document.authForm.authName.value = "";
	document.authForm.authName.disabled = false;
	document.authForm.authReg.disabled = false;
	document.authForm.sub.value = "Отправить";
}


//Работа с cookie
function setCookie(name, value, options) {
	console.log("dd");
  options = options || {};

  var expires = options.expires;

  if (typeof expires == "number" && expires) {
    var d = new Date();
    d.setTime(d.getTime() + expires * 1000);
    expires = options.expires = d;
  }
  if (expires && expires.toUTCString) {
    options.expires = expires.toUTCString();
  }

  value = encodeURIComponent(value);

  var updatedCookie = name + "=" + value;

  for (var propName in options) {
    updatedCookie += "; " + propName;
    var propValue = options[propName];
    if (propValue !== true) {
      updatedCookie += "=" + propValue;
    }
  }

  document.cookie = updatedCookie;
}

function getCookie(name) {
  var matches = document.cookie.match(new RegExp(
    "(?:^|; )" + name.replace(/([\.$?*|{}\(\)\[\]\\\/\+^])/g, '\\$1') + "=([^;]*)"
  ));
  return matches ? decodeURIComponent(matches[1]) : undefined;
}

function deleteCookie(name) {
  setCookie(name, "", {
    expires: -1
  })
}
