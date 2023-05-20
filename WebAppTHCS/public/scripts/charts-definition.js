// Create the charts when the web page loads
window.addEventListener('load', onload);

Highcharts.setOptions({
  global: {
      useUTC: false
  }
});

// Load dữ liệu

function onload(event){
  chartT = createTemperatureChart();
  chartH = createHumidityChart();
  chartL = createLightChart();
  chartS = createSoundChart();
}

// Create Temperature Chart
function createTemperatureChart() {
  var chart = new Highcharts.Chart({
    
    chart:{ 
      renderTo:'chart-temperature',
      type: 'spline' 
    },
    series: [
      {
        name: 'DHT11'
      }
    ],
    title: { 
      text: undefined
    },
    plotOptions: {
      line: { 
        animation: false,
        dataLabels: { 
          enabled: true 
        }
      }
    },
    xAxis: {
      type: 'datetime',
      dateTimeLabelFormats: { second: '%H:%M:%S' }
    },
    yAxis: {
      title: { 
        text: 'Nhiệt độ (*C)' 
      }
    },
    credits: { 
      enabled: false 
    }
  });
  return chart;
}

// Create Humidity Chart
function createHumidityChart(){
  var chart = new Highcharts.Chart({
    chart:{ 
      renderTo:'chart-humidity',
      type: 'spline'  
    },
    series: [{
      name: 'DHT11'
    }],
    title: { 
      text: undefined
    },    
    plotOptions: {
      line: { 
        animation: false,
        dataLabels: { 
          enabled: true 
        }
      },
      series: { 
        color: '#50b8b4' 
      }
    },
    xAxis: {
      type: 'datetime',
      dateTimeLabelFormats: { second: '%H:%M:%S' }
    },
    yAxis: {
      title: { 
        text: 'Độ ẩm (%)' 
      }
    },
    credits: { 
      enabled: false 
    }
  });
  return chart;
}

// Create Light Chart
function createLightChart() {
  var chart = new Highcharts.Chart({
    chart:{ 
      renderTo:'chart-light',
      type: 'spline'  
    },
    series: [{
      name: 'Light Sensor'
    }],
    title: { 
      text: undefined
    },    
    plotOptions: {
      line: { 
        animation: false,
        dataLabels: { 
          enabled: true 
        }
      },
      series: { 
        color: '#A62639' 
      }
    },
    xAxis: {
      type: 'datetime',
      dateTimeLabelFormats: { second: '%H:%M:%S' }
    },
    yAxis: {
      title: { 
        text: 'Cường độ (cd)' 
      }
    },
    credits: { 
      enabled: false 
    }
  });
  return chart;
}

// Create Sound Chart
function createSoundChart() {
    var chart = new Highcharts.Chart({
      chart:{ 
        renderTo:'chart-sound',
        type: 'spline'  
      },
      series: [{
        name: 'Sound Sensor'
      }],
      title: { 
        text: undefined
      },    
      plotOptions: {
        line: { 
          animation: false,
          dataLabels: { 
            enabled: true 
          }
        },
        series: { 
          color: '#C08080' 
        }
      },
      xAxis: {
        type: 'datetime',
        dateTimeLabelFormats: { second: '%H:%M:%S' }
      },
      yAxis: {
        title: { 
          text: 'Độ lớn (dB)' 
        }
      },
      credits: { 
        enabled: false 
      }
    });
    return chart;
  }
  