var xmlHttp = new XMLHttpRequest();
xmlHttp.open("GET", "/state", true);
xmlHttp.onreadystatechange = function (aEvt) {
  if(xmlHttp.status != 200){
    console.log("ERROR: [" + xmlHttp.status+"] " + xmlHttp.responseText);
  }
  else{
    var state = JSON.parse(xmlHttp.responseText);
    // var maxSamples = parseInt(document.getElementById("maxSamplesField").value);
    document.getElementById("mode").value = state.mode;
    var freqM = Math.floor(state.freq/1000000);
    var freqK = Math.floor((state.freq - freqM*1000000)/1000);
    var freq = state.freq - freqM*1000000 - freqK*1000;
    document.getElementById("freqM").value = freqM;
    document.getElementById("freqK").value = freqK;
    document.getElementById("freq").value = freq;
    document.getElementById("phase").value = state.phase;
    document.getElementById("gain").value = state.gain;
  }
}
xmlHttp.send();


// EVENT LISTENERS
document.getElementById('stateButton').addEventListener('click', function (e) {
  var sendStr = "/state?";

  var param = document.getElementById("mode").value;
  if(param != "")
    sendStr += "mode=" + param + "&";

  if(document.getElementById("freqM").value == "")
    document.getElementById("freqM").value = "0";
  if(document.getElementById("freqK").value == "")
    document.getElementById("freqK").value = "0";
  if(document.getElementById("freq").value == "")
    document.getElementById("freq").value = "0";

  var freq = parseInt(document.getElementById("freqM").value)*1000000;
  freq += parseInt(document.getElementById("freqK").value)*1000;
  freq += parseFloat(document.getElementById("freq").value);
  param = freq;
  sendStr += "freq=" + param + "&";

  param = parseFloat(document.getElementById("phase").value);
  sendStr += "phase=" + param + "&";

  param = parseFloat(document.getElementById("gain").value);
  sendStr += "gain=" + param;

  var xmlHttp = new XMLHttpRequest();
  xmlHttp.open("GET", sendStr, true);
  xmlHttp.onreadystatechange = function (aEvt) {
    if(xmlHttp.status != 200){
      console.log("ERROR: [" + xmlHttp.status+"] " + xmlHttp.responseText);
    }
  }

  xmlHttp.send();
});
////////////////