import config from '../config/network.json' assert { type: 'json' };

document.getElementById("apssid").value = config["apssid"];
document.getElementById("appsk").value = config["appsk"];
document.getElementById("stassid").value = config["stassid"];
document.getElementById("stapsk").value = config["stapsk"];
document.getElementById("host").value = config["host"];

document.getElementById('saveButton').addEventListener('click', function (e) {
  var saveStr = "/network?";

  var param = document.getElementById("apssid").value;
  if(param != "")
    saveStr += "apssid=" + param + "&";

  param = document.getElementById("appsk").value;
  if(param != "")
    saveStr += "appsk=" + param + "&";

  param = document.getElementById("stassid").value;
  if(param != "")
    saveStr += "stassid=" + param + "&";

  param = document.getElementById("stapsk").value;
  if(param != "")
    saveStr += "stapsk=" + param + "&";

  param = document.getElementById("host").value;
  if(param != "")
    saveStr += "host=" + param;

  var xmlHttp = new XMLHttpRequest();
  xmlHttp.open("GET", saveStr, true);
  xmlHttp.onreadystatechange = function (aEvt) {
    if(xmlHttp.status == 200)
        window.location.reload();
    else
        console.log("ERROR: [" + xmlHttp.status+"] " + xmlHttp.responseText);

  }

  xmlHttp.send();
});

document.getElementById('restartButton').addEventListener('click', function (e) {
  
    var xmlHttp = new XMLHttpRequest();
    xmlHttp.open("GET", "/restart", true);
    xmlHttp.onreadystatechange = function (aEvt) {
      if(xmlHttp.status == 200)
          window.location.reload();
      else
          console.log("ERROR: [" + xmlHttp.status+"] " + xmlHttp.responseText);
  
    } 

    xmlHttp.send();
  });