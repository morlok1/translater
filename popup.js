document.addEventListener('DOMContentLoaded', function () 
{
  getStart();
});

function getStart()
{
  document.translateForm.translateText.addEventListener('input', function ()
  {
	getTranslate();
  });
  
  document.addEventListener('submit', function()
  {
	 event.preventDefault();
  });

}

function getTranslate()
{
	var key = "dict.1.1.20170318T103102Z.6bf3bd6d3840a4a3.e5ee46ccab2ac06111418309c4034544d15590f4";
	var langDirect = "en-ru";
	var url = "https://dictionary.yandex.net/api/v1/dicservice/lookup?" 
	+ "key=" + key
	+ "&lang=" + langDirect
	+ "&text=" + document.translateForm.translateText.value;
	var xhr = new XMLHttpRequest();

	xhr.open('GET', url, false);
	xhr.send();
	if (xhr.status != 200) 
	{
		document.getElementById('translateResult').innerHTML = "Some thing wrong...";
	} 
	else 
	{
		var xmlReq = xhr.responseXML;
		var enWord = xmlReq.getElementsByTagName("text")[0].innerHTML;
		var ruWord = xmlReq.getElementsByTagName("text")[1].innerHTML;
		document.getElementById('translateResult').innerHTML = ruWord;
	}
}			