var fs = require("fs");
var file = "./satellites.db";
var exists = fs.existsSync(file);

var sqlite3 = require('sqlite3').verbose();
var db = new sqlite3.Database(file);
var settings = require('./settings.json');

var populate = false;

db.serialize(function() {
  if(!exists){
    console.log("Creating DB file");
    populate = true;
  } else {
    console.log("DB already exists. Checking for updates...");
  }
  db.run("CREATE TABLE IF NOT EXISTS satellites (id TEXT, noradCatId INTEGER, dispName TEXT, tle1 TEXT, tle2 TEXT, downlink INTEGER, radioInterface TEXT)");
  if(populate) {
    var satellites = settings.satellites;
    for(var key in satellites){
      var sat = satellites[key];
      console.log(key);
      db.run('INSERT INTO satellites VALUES (?,?,?,?,?,?,?)', key, sat.NORAD_CAT_ID, sat.dispName, sat.tle1, sat.tle2, sat.downlink, sat.radioInterface);
    }
  }
});

module.exports.getNoradId = function getNoradId(id, callback){
  console.log(id);
  db.get('SELECT noradCatId FROM satellites WHERE id=?', id, callback);
}

module.exports.updateNoradId = function updateNoradId(catId, newtle1, newtle2, callback){
  db.run('UPDATE satellites SET tle1=? tle2=? WHERE noradCatId=?', newtle1, newtle2, catId, callback);
}

module.exports.getSatellite = function getSatellite(id, callback){
  console.log('id');
  db.get('SELECT * FROM satellites WHERE id=?', id, callback);
}

//db.close();

/*module.exports.checkActive = function checkActive(room, sid, callback){
  db.get('SELECT * FROM ' + room + ' WHERE sid=?', sid, callback);
}
module.exports.signInStudent = function signInStudent(room, student){
  var date = new Date().toLocaleString();
  console.log(date);
  db.run('INSERT INTO ' + room + ' VALUES (?,?,?,?,?,?,?,?)', student.sid, student.firstName, student.lastName, student.grade, student.team, date, student.device, student.fields);
}
module.exports.checkActiveDevice = function checkActiveDevice(room, device, callback){
  db.get('SELECT * FROM ' + room + ' WHERE device=?', device, callback);
}
module.exports.signOutStudent = function signOutStudent(room, student, callback) {
  db.run('DELETE FROM ' + room + ' WHERE sid=?', student.sid);
  callback(); //?
}

module.exports.getAllActive = function getAllActive(room, callback){
  db.all('SELECT * FROM ' + room, callback);
}*/
