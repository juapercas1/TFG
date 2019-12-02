
function renderChartEMG(data, labels, idChart){
  var ctx = document.getElementById(idChart).getContext('2d');
  var emgChart = new Chart(ctx, {
    type: 'line',
    data: {
      labels: labels,
      datasets: [{
        label: 'EMG',
        data: data,
      }]
    },
  });

}

function renderChartACC(data, labels, idChart){
  var ctx = document.getElementById(idChart).getContext('2d');
  var emgChart = new Chart(ctx, {
    type: 'line',
    data: {
      labels: labels,
      datasets: [{
        label: 'Pasos',
        data: data,
      }]
    },
  });

}

function getData(response){


  var values = response.data.map(item => {
      return item.valor

  })
  var dates = response.data.map(item => {
    return item.fecha

  })
  return {values, dates}
}

window.setInterval(() => {
  axios({
  url: 'http://localhost:8080/api/fechas/emg',
  method: 'get'
  }).then(response1 => {
    let emgData = getData(response1)
    renderChartEMG(emgData.values, emgData.dates, "chart1");
  })

axios({
  url: 'http://localhost:8080/api/fechas/acc',
  method: 'get'
  }).then(response2 => {
    let accData = getData(response2)
    renderChartACC(accData.values, accData.dates, "chart2");

  })

}, 3000)
