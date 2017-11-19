var express = require('express');
var app = express();
var http = require('http').Server(app);
var io = require('socket.io')(http);
var request = require('request');
var fs = require('fs');

var db = require('./db.js');

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
	console.log('a user connected' + "\n");

  socket.emit('welcome', { message: 'Welcome!', id: socket.id });

  socket.on('i am client', console.log);

  socket.on('update time', function() {
    console.log("attempt to update the time");
  })

  socket.on('disconnect', function() { console.log('disconnected'); });

});
