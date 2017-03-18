function getTranslate()
{
	console.log("---------------------");
	console.log("Start working...");
	var key = "dict.1.1.20170318T103102Z.6bf3bd6d3840a4a3.e5ee46ccab2ac06111418309c4034544d15590f4";
	
	var url = "https://dictionary.yandex.net/api/v1/dicservice/lookup?" 
	+ "key=" + key
	+ "&lang=en-ru"
	+ "&text=" + document.f.text.value;
	var xhr = new XMLHttpRequest();

	xhr.open('GET', url, false);
	console.log("Send request: " + document.f.text.value);
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
		document.form.sub.value = ruWord;
		console.log("Get translate: " + ruWord);
	}
	console.log("---------------------");
}			