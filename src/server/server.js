// server.js

// call the packages we need
var express    = require('express');        // call express
var app        = express();                 // define our app using express
var bodyParser = require('body-parser');
var cors = require('cors');
var influx = require('influx')

app.use(cors());

// configure app to use bodyParser()
// this will let us get the data from a POST

app.use(bodyParser.urlencoded({ extended: true }));
app.use(bodyParser.json());

var port = process.env.PORT || 12345;        // set our port

// ROUTES FOR OUR API
// =============================================================================
var router = express.Router();              // get an instance of the express Router



const influx_client = new influx.InfluxDB({
    host: '195.148.126.71',
    database: 'iothon_db',
    schema: [
      {
        measurement: influx.FieldType.STRING,
        fields: {
          value: influx.FieldType.STRING
        },
        tags: [
          'location'
        ]
      }
    ]
  })


router.get('/temperature', function(req, res) {
    influx_client.query('SELECT mean("value") FROM "temperature" WHERE ("location" = \'myteam\') AND time >= now() - 6h GROUP BY time(1m) fill(null)').then(result => {
        res.json(result);
    })
});
router.get('/humidity', function(req, res) {
    influx_client.query('SELECT mean("value") FROM "humidity" WHERE ("location" = \'myteam\') AND time >= now() - 6h GROUP BY time(1m) fill(null)').then(result => {
        res.json(result);
    })
});
router.get('/co2', function(req, res) {
    influx_client.query('SELECT mean("value") FROM "co2" WHERE ("location" = \'myteam\') AND time >= now() - 6h GROUP BY time(1m) fill(null)').then(result => {
        res.json(result);
    })
});
router.get('/pressure', function(req, res) {
    influx_client.query('SELECT mean("value") FROM "pressure" WHERE ("location" = \'myteam\') AND time >= now() - 6h GROUP BY time(1m) fill(null)').then(result => {
        res.json(result);
    })
});




// REGISTER OUR ROUTES -------------------------------
// all of our routes will be prefixed with /api
app.use('/api', router);

// START THE SERVER
// =============================================================================
app.listen(port);
console.log('Magic happens on port ' + port);