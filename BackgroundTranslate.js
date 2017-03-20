
/*
chrome.browserAction.onClicked.addListener(function(tab) {
  // No tabs or host permissions needed!
  console.log('Turning ' + tab.url + ' red!');
  chrome.tabs.executeScript({
    code: 'document.body.style.backgroundColor="red"'
  });
});
*/

document.addEventListener('DOMContentLoaded', function () {
  alert("r");
  //getStart();
});

function getStart()
{
	alert("ddd");
	//var d = document.myForm.sub;
	document.addEventListener('click', function () {
  alert("ddd")
});
	document.addEventListener('select', function () {
  getSelectionText();
});

}

function getSelectionText() 
{
  var txt = '';
  if (txt = window.getSelection)
  {	  // Не IE, используем метод getSelection
    txt = window.getSelection().toString();
  } 
  else 
  { // IE, используем объект selection
    txt = document.selection.createRange().text;
  }
  alert(txt);
}

function getTranslate()
{
	var key = "dict.1.1.20170318T103102Z.6bf3bd6d3840a4a3.e5ee46ccab2ac06111418309c4034544d15590f4";
	
	var url = "https://dictionary.yandex.net/api/v1/dicservice/lookup?" 
	+ "key=" + key
	+ "&lang=en-ru"
	+ "&text=time"
	var xhr = new XMLHttpRequest();

	xhr.open('GET', url, false);
	xhr.send();
	if (xhr.status != 200) 
	{
		console.log( xhr.status + ': ' + xhr.statusText );
	} 
	else 
	{
		var xmlReq = xhr.responseXML;
		var enWord = xmlReq.getElementsByTagName("text")[0].innerHTML;
		var ruWord = xmlReq.getElementsByTagName("text")[1].innerHTML;
		alert(ruWord);
	}
}