MAX_SAMPLES = 100;
PERIOD_MS = 1000;

// FUNCTIONS

// Fill elements with initialization
function fillFields() {
  var xmlHttp = new XMLHttpRequest();
  xmlHttp.open("GET", "/state", true);
  xmlHttp.onreadystatechange = function (aEvt) {
    if(xmlHttp.status != 200){
      console.log("ERROR: [" + xmlHttp.status+"] " + xmlHttp.responseText);
    }
    else{
      var state = JSON.parse(xmlHttp.responseText);
      document.getElementById("freq").value = state.freq;
    }
  }
  xmlHttp.send();
}

// Create analog chart
var analogChart = new Chart(document.getElementById('analog'), {
  // The type of chart we want to create
  type: 'scatter',
  // The data for our dataset
  data: {
    datasets: [{
      label: 'Analog',
      backgroundColor: 'rgb(0,0,0,0)',
      borderColor: 'rgb(255, 99, 132)',
      data: [],
      showLine:true
    }]
  },
  // Configuration options go here
  options: {
    maintainAspectRatio: false,
    title: {
      display: true,
      text: 'Analog input'
    },
    legend: {
      display: false
    },
    scales: {
      xAxes: [{
        id:"x-axis",
        ticks: {
          minRotation:0,
          maxRotation:0
        }
      }],
      yAxes: [{
        ticks: {
          beginAtZero:true
        }
      }]
    },
    tooltips: {
      intersect: false
    },
    animation: {
      duration: 0
    },
    hover: {
      animationDuration: 0
    },
    
    // Zoom plugin options
    plugins: {
      zoom: {
        pan: {
          enabled: true,
          mode: 'y',
        },
        zoom: {
          enabled: true,
          mode: 'y',
        }
      }
    },
    
    onClick: function() {this.resetZoom()}
  }
});

// UTILITY FUNCTIONS

// Compute a string with min/max/average on the given array of (x, y) points
function computeStats(sampleArray) {
  var minY = Number.MAX_SAFE_INTEGER, maxY = 0, avgY = 0;       
  var prevX = 0;
  sampleArray.forEach(function (point, index) {
    if (point.y < minY) minY = point.y;
    if (point.y > maxY) maxY = point.y;
    if (prevX > 0) avgY = avgY + point.y * (point.x - prevX); // avg is weighted: samples with a longer period weight more
    prevX = point.x;
  });
  avgY = avgY / (prevX - sampleArray[0].x);
  return "min: " + minY + ", max: " + maxY + ", avg: " + Math.floor(avgY);
}


// PERIODIC FETCH OF NEW DATA
var running = true;
var configDiv = document.getElementById("config");

function fetchNewData() {
  if (!running) return;
  var fetchStartMs = Date.now();
  var xmlHttp = new XMLHttpRequest();
  var maxSamples = MAX_SAMPLES;
  xmlHttp.onload = function() {
    var processingDurationMs;
    if(xmlHttp.status != 200) {
      // Server error
      configDiv.style.backgroundColor = "red";
      console.log("GET ERROR: [" + xmlHttp.status+"] " + xmlHttp.responseText);
      processingDurationMs = new Date() - fetchStartMs;
    }
    else {
      configDiv.style.backgroundColor = "white";
      // Response is e.g.: {"time":963074, "heap":47160, "analog":60, "gpioMask":16447, "gpioData":16405}
      var stateData = JSON.parse(xmlHttp.responseText);
      var x = stateData.time;
      
      // Add point to analog chart
      data = analogChart.data.datasets[0].data;
      data.push({x: stateData.time, y: stateData.analog});
      // Limit its length to maxSamples
      data.splice(0, data.length - maxSamples);
      // Put stats in chart title
      analogChart.options.title.text = "Analog input (" + computeStats(data) + ") - use mouse wheel or pinch to zoom";
      // Remember smallest X
      var minX = data[0].x;
      
      // Set origin of X axis to minimum X value
      if (analogChart.options.scales.xAxes[0].ticks.min === undefined || analogChart.options.scales.xAxes[0].ticks.min < minX) {
        analogChart.options.scales.xAxes[0].ticks.min = minX;
      }
      analogChart.update();
      // Check if we can keep up with this fetch rate, or increase interval by 50ms if not.        
      processingDurationMs = new Date() - fetchStartMs;
      // periodField.style.backgroundColor = (processingDurationMs > parseInt(periodField.value))?"#FF7777":"#FFFFFF";
    
      document.getElementById("analogValue").textContent = stateData.analog.toPrecision(4);
    }
    
    // Schedule next call with the requested interval minus the time needed to process the previous call
    // Note that contrary to setInterval, this technique guarantees that we will never have 
    // several fetch in parallel in case we can't keep up with the requested interval
    var waitingDelayMs = PERIOD_MS - processingDurationMs//parseInt(periodField.value) - processingDurationMs;
    if (running) {
      window.setTimeout(fetchNewData, (waitingDelayMs > 0)?waitingDelayMs:0);
    }
  };    
  xmlHttp.onerror = function(e) {
    // Ajax call error
    configDiv.style.backgroundColor = "red";
    console.log("ERROR: [" + xmlHttp.status+"] " + xmlHttp.responseText);
    // Also schedule next call in case of error
    var waitingDelayMs = PERIOD_MS;//parseInt(periodField.value);
    window.setTimeout(fetchNewData, (waitingDelayMs > 0)?waitingDelayMs:0);      
  };
  xmlHttp.open("GET", "/state", true);
  xmlHttp.send();
}


// EVENT LISTENERS

// Set button
document.getElementById('setButton').addEventListener('click', function (e) {
  var sendStr = "/state?";

  sendStr += "freq=" + parseFloat(document.getElementById("freq").value);

  var xmlHttp = new XMLHttpRequest();
  xmlHttp.open("GET", sendStr, true);
  xmlHttp.onreadystatechange = function (aEvt) {
    if(xmlHttp.status != 200){
      console.log("ERROR: [" + xmlHttp.status+"] " + xmlHttp.responseText);
    }
  }

  xmlHttp.send();
});

// Start/pause button
document.getElementById('startPauseButton').addEventListener('click', function (e) {    
  running = !running;
  if (running) {
    document.getElementById('startPauseButton').textContent = "PAUSE";
    document.getElementById('clearButton').disabled = true;
    fetchNewData();
  }
  else {
    document.getElementById('startPauseButton').textContent = "START";
    document.getElementById('clearButton').disabled = false;
  }
});
// Clear button
document.getElementById('clearButton').addEventListener('click', function (e) {
  analogChart.data.datasets[0].data.length = 0;
  analogChart.update(); 
});

////////////////

fillFields();
fetchNewData();