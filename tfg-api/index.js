var express = require("express"),
    app = express(),
    bodyParser  = require("body-parser"),
    methodOverride = require("method-override");

var moment = require("moment")

var mysql = require('mysql');

//Conexión a la base de datos
var con = mysql.createConnection({
  host: "localhost",
  user: "root",
  password: "root",
  database: "TFG"
});

con.connect(function(err) {
  if (err) throw err;
  console.log("Connected to MySql");
});
//creación de las directivas get y post para su posterior uso

app.use(bodyParser.urlencoded({ extended: false }));
app.use(bodyParser.json());
app.use(methodOverride());

var router = express.Router();

router.get('/api/fechas/emg', function(req, res) {
  con.query("SELECT * FROM regfechas WHERE idSensor = 'EMG1'", function (err, result, fields) {
    if (err) throw err;
    console.log(result);
    res.send(result);
  });
});

router.get('/api/fechas/acc', function(req, res) {
  con.query("SELECT * FROM regfechas WHERE idSensor = 'ACC1'", function (err, result, fields) {
    if (err) throw err;
    console.log(result);
    res.send(result);
  });
});

router.post('/api/fechas', function(req, res) {
  let idSensor = req.body.idSensor
  let valor = req.body.valor

  let query = `INSERT INTO regfechas (idSensor, valor, fecha) VALUES ('${idSensor}', '${valor}', now())`

  con.query(query, function(err, result) {
    if (err) throw err;
    res.send(result)
  })
})


app.use(router);




app.listen(8080, function() {
  console.log("Node server running on http://localhost:8080");
});
