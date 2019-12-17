var express = require('express');
var app = express();
var http = require('http').Server(app);
var io = require('socket.io')(http);
var fs = require('fs');

var pointer = require('./serial.js');
var db = require('./db.js');

var spacetrack = require('spacetrack');
spacetrack.login({
  username: '',
  password: ''
  // lol no credentials
});


pointer.sendTime();
pointer.attachParser();

app.use(express.static('public'));

app.engine('html', function(filePath, opts, callback){
  fs.readFile(filePath, function(err, content) {
    if(err) return callback(new Error(err));
    var rendered = content.toString();
    return callback(null, rendered);
  });
});

app.get('/', function (req, res){
  res.render(__dirname + '/index.html');
});

var server = http.listen(3000, function() {
  var host = server.address().address;
  var port = server.address().port;

  console.log('App listening at http://%s%s', host, port);
});

io.on('connection', function(socket) {
  var satdata = [];
  console.log('a user connected' + "\n");
  db.getSatellite("ISS", function(err, row){
    console.log(row);
    satdata[0] = row;
    db.getSatellite("HST", function(err, row){
      console.log(row);
      satdata[1] = row;
      db.getSatellite("NOAA15", function(err, row){
        console.log(row);
        satdata[2] = row;
        socket.emit('welcome', { message: satdata, id: socket.id });
      });
    });
  });
  socket.on('i am client', console.log);

  socket.on('update time', function() {
    console.log("attempt to update the time");
    pointer.sendTime();
    var date = new Date().toLocaleString();
    console.log(date);
  });

  socket.on('update TLE', function() {
    console.log("yo");
    getAllTLEs();
    // This is to manually update the TLE for all the satellites
    // ask CelesTrack for the new TLEs
    // save the new TLEs in the DB
  });

  socket.on('switch satellite', function(msg){
    // tell the mbed processor to follow a different satellite
    db.getSatellite(msg.data, function(err, row) {
      //console.log('Switching satellite to: ' + msg.data + '\n ONE: ' + row.tle1 + '\n TWO: ' + row.tle2);
      pointer.sendTLE(row.tle1, row.tle2);
    });
  })


  socket.on('disconnect', function() { console.log('disconnected'); });

});

function getAllTLEs(){
  //get all the norad IDs and comma delimit into string
  var idList = "";
  console.log('idlist');
  db.getNoradId("ISS", function(err, row) {
    console.log('iss');
    console.log(row);
    idList+=row.noradCatId +",";
    db.getNoradId("HST", function(err, row) {
      console.log('hst');
      console.log(row);
      idList+=row.noradCatId +",";
      db.getNoradId("NOAA15", function(err, row) {
        console.log('noaa');
        console.log(row);
        idList+=row.noradCatId;
        console.log(idList);
        getTLE(idList);
      });
    });
  });
}

function getTLE(noradIdList){
  spacetrack.get({
    type: 'tle_latest',
    query: [
      {field:'NORAD_CAT_ID', condition: noradIdList},
      {field:'ORDINAL', condition: '1'},
    ],
    predicates: [
      'OBJECT_NAME',
      'NORAD_CAT_ID',
      'TLE_LINE0',
      'TLE_LINE1',
      'TLE_LINE2'
      ]
  })
  .then(function(result) {
    console.log( result );
    db.updateNoradId(result[0].catalogNumber, result[0].tle[1], result[0].tle[2], function(err){
      console.log('update ' + result[0].name);
      db.updateNoradId(result[1].catalogNumber, result[1].tle[1], result[1].tle[2], function(err){
        console.log('update ' + result[1].name);
        db.updateNoradId(result[2].catalogNumber, result[2].tle[1], result[2].tle[2], function(err){
          console.log('update ' + result[2].name);
          console.log('done');
        })
      });
    });
  }, function(err) {
    console.error('error', err.stack);
  });
}
