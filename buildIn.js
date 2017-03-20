//var script = document.createElement('script');

//script.src = chrome.extension.getURL("BackgroundTranslate.js");
//alert(script.src)
//if (!document.head.appendChild(script))
//	alert("WTF")

var New_js = document.createElement('script');
    New_js.type = 'text/javascript';
    New_js.src = chrome.extension.getURL("BackgroundTranslate.js");
    document.getElementsByTagName('head')[0].appendChild(New_js);
