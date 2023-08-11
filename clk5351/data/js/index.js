OUTPUTS_SIZE = 3;

function fillFields(chan) {
  var xmlHttp = new XMLHttpRequest();
  xmlHttp.open("GET", "/state", true);
  xmlHttp.onreadystatechange = function (aEvt) {
    if(xmlHttp.status != 200){
      console.log("ERROR: [" + xmlHttp.status+"] " + xmlHttp.responseText);
    }
    else{
      var state = JSON.parse(xmlHttp.responseText);

      document.getElementById("output").value = chan;

      var freqM = Math.floor(state.output[chan].freq/1000000);
      var freqK = Math.floor((state.output[chan].freq - freqM*1000000)/1000);
      var freq = state.output[chan].freq - freqM*1000000 - freqK*1000;
      document.getElementById("freqM").value = freqM;
      document.getElementById("freqK").value = freqK;
      document.getElementById("freq").value = freq;

      document.getElementById("phase").value = state.output[chan].phase;
      
      var chk = state.output[chan].enable == "1" ? true : false;
      document.getElementById("enable").checked = chk;

      chk = state.output[chan].invert == "1" ? true : false;
      document.getElementById("invert").checked = chk;

      document.getElementById("idrive").value = state.output[chan].idrive;

    }
  }
  xmlHttp.send();
}


// EVENT LISTENERS
document.getElementById('output').addEventListener('change', function (e) {
  fillFields(document.getElementById("output").value);
});

document.getElementById('setButton').addEventListener('click', function (e) {
  var sendStr = "/state?";

  var param = document.getElementById("output").value;
  if(param != "")
    sendStr += "output=" + param + "&";

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
  sendStr += "phase=" + param  + "&";

  param = document.getElementById("enable").checked ? "1" : "0";
  sendStr += "enable=" + param  + "&";

  param = document.getElementById("invert").checked ? "1" : "0";
  sendStr += "invert=" + param  + "&";

  param = document.getElementById("idrive").value;
  sendStr += "idrive=" + param;

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

fillFields(0);