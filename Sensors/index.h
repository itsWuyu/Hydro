const char index_html[] PROGMEM = R"rawliteral(


<!DOCTYPE HTML><html>

<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <script src="https://code.highcharts.com/highcharts.js"></script>
  <link rel="shortcut icon" type="image/x-icon" href="https://img.icons8.com/color/48/000000/potted-plant--v2.png">
  <link rel="preconnect" href="https://fonts.gstatic.com">
  <link href="https://fonts.googleapis.com/css2?family=Cousine:wght@700&family=Roboto:wght@700&display=swap" rel="stylesheet">
  <link href="https://cdn.jsdelivr.net/npm/bootstrap@5.0.0-beta2/dist/css/bootstrap.min.css" rel="stylesheet" integrity="sha384-BmbxuPwQa2lc/FVzBcNJ7UAyJxM6wuqIj61tLrc4wSX0szH/Ev+nYRRuWlolflfl" crossorigin="anonymous">
  <style>
    :root {
    --main-bg-color: #ffffff;
    }
    body {  
    
      max-width: 100%;
      height: 100vh;
      background-color: var(--main-bg-color);
      
    }
    h2 {
      font-family: 'Roboto', sans-serif;
      font-size: 2.5rem;
      text-align: center;
      color: black;
      text-transform: uppercase;
      font-weight: bold;
      padding-top: 2%;
     
    }
  </style>
</head>
<body>
  <h2>Hydro-Farm</h2>
  <div class="container" style="margin-top: 20px;">
    <div class="row">
  <div id="chart-temperature" class="col-md-6"></div>
  <div id="chart-humidity" class="col-md-6"></div>
    </div>
  </div>
  <div class="container" style="margin-top: 5%;">
    <div class="row">
  <div id="chart-watertemp" class="col-md-6"></div>
  <div id="chart-waterlevel" class="col-md-6"></div>
    </div>
  </div>
   <div class="container" style="margin-top: 5%;">
    <div class="row">
  <div id="chart-co2" class="col-md-6"></div>
  <div id="" class="col-md-6"></div>
    </div>
  </div>
</body>
<script>
var chartT = new Highcharts.Chart({
  chart:{ renderTo : 'chart-temperature' },
  title: { text: 'DHT11 Temperature', },
  series: [{
    showInLegend: false,
    data: []
  }],
  plotOptions: {
    line: { animation: true,
      dataLabels: { enabled: true }
    },
    series: { color: '#1CAC78' }
  },
  xAxis: { type: 'datetime',
  lineColor: 'black',
    dateTimeLabelFormats: { second: '%H:%M:%S' }
  },
  yAxis: {
    title: { text: 'Temperature (Celsius)' }
  
  },
  credits: { enabled: false }
});
setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      var x = (new Date()).getTime()+ 3600000, // + 3600000 for UTC +01
          y = parseFloat(this.responseText);
      //console.log(this.responseText);
      if(chartT.series[0].data.length > 40) {
        chartT.series[0].addPoint([x, y], true, true, true);
      } else {
        chartT.series[0].addPoint([x, y], true, false, true);
      }
    }
  };
  xhttp.open("GET", "/temperature", true);
  xhttp.send();
}, 30000 ) ;

var chartH = new Highcharts.Chart({
  chart:{ renderTo:'chart-humidity' },
  title: { text: 'DHT11 Humidity' },
  series: [{
    showInLegend: false,
    data: []
  }],
  plotOptions: {
    line: { animation: false,
      dataLabels: { enabled: true }
    },
    series: { color: '#1CAC78' }
  },
  xAxis: {
    type: 'datetime',
    lineColor: 'black',
    dateTimeLabelFormats: { second: '%H:%M:%S' }
  },
  yAxis: {
    title: { text: 'Humidity (%)' }
  },
  credits: { enabled: false }
});
setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      var x = (new Date()).getTime()+ 3600000,
          y = parseFloat(this.responseText);
      //console.log(this.responseText);
      if(chartH.series[0].data.length > 40) {
        chartH.series[0].addPoint([x, y], true, true, true);
      } else {
        chartH.series[0].addPoint([x, y], true, false, true);
      }
    }
  };
  xhttp.open("GET", "/humidity", true);
  xhttp.send();
}, 30000 ) ;

var chartP = new Highcharts.Chart({
  chart:{ renderTo : 'chart-watertemp' },
  title: { text: 'DS18B20 Watertemp' },
  series: [{
    showInLegend: false,
    data: []
  }],
  plotOptions: {
    line: { animation: false,
      dataLabels: { enabled: true }
    },
    series: { color: '#1CAC78' }
  },
  xAxis: { type: 'datetime',
    lineColor: 'black',
    dateTimeLabelFormats: { second: '%H:%M:%S' }
  },
  yAxis: {
    title: { text: 'Temperature (Celsius)' }
  
  },
  credits: { enabled: false }
});
setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      var x = (new Date()).getTime()+ 3600000,
          y = parseFloat(this.responseText);
      //console.log(this.responseText);
      if(chartP.series[0].data.length > 40) {
        chartP.series[0].addPoint([x, y], true, true, true);
      } else {
        chartP.series[0].addPoint([x, y], true, false, true);
      }
    }
  };
  xhttp.open("GET", "/watertemp", true);
  xhttp.send();
}, 30000 ) ;

var chartL = new Highcharts.Chart({
  chart:{ renderTo : 'chart-waterlevel' },
  title: { text: 'Water Level' },
  series: [{
    showInLegend: false,
    data: []
  }],
  plotOptions: {
    line: { animation: false,
      dataLabels: { enabled: true }
    },
    series: { color: '#1CAC78' }
  },
  xAxis: { type: 'datetime',
    lineColor: 'black',
    dateTimeLabelFormats: { second: '%H:%M:%S' }
  },
  yAxis: {
    title: { text: 'Level' }
  
  },
  credits: { enabled: false }
});
setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      var x = (new Date()).getTime()+ 3600000,
          y = parseFloat(this.responseText);
      //console.log(this.responseText);
      if(chartL.series[0].data.length > 40) {
        chartL.series[0].addPoint([x, y], true, true, true);
      } else {
        chartL.series[0].addPoint([x, y], true, false, true);
      }
    }
  };
  xhttp.open("GET", "/waterlevel", true);
  xhttp.send();
}, 30000 ) ;

var chartC = new Highcharts.Chart({
  chart:{ renderTo : 'chart-co2' },
  title: { text: 'CO2 Wert' },
  series: [{
    showInLegend: false,
    data: []
  }],
  plotOptions: {
    line: { animation: false,
      dataLabels: { enabled: true }
    },
    series: { color: '#1CAC78' }
  },
  xAxis: { type: 'datetime',
    lineColor: 'black',
    dateTimeLabelFormats: { second: '%H:%M:%S' }
  },
  yAxis: {
    title: { text: 'ppm' }
  
  },
  credits: { enabled: false }
});
setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      var x = (new Date()).getTime()+ 3600000,
          y = parseFloat(this.responseText);
      //console.log(this.responseText);
      if(chartC.series[0].data.length > 40) {
        chartC.series[0].addPoint([x, y], true, true, true);
      } else {
        chartC.series[0].addPoint([x, y], true, false, true);
      }
    }
  };
  xhttp.open("GET", "/co2", true);
  xhttp.send();
}, 30000 ) ;





</script> 
</html>)rawliteral";
