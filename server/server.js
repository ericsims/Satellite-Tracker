var express = require('express');
var app = express();
var http = require('http').Server(app);
var io = require('socket.io')(http);
var request = require('request');
var fs = require('fs');

app.use(express.static('public'));

app.get('/', function (req, res){
  var page = "<li> hello world </li>";
  res.send(page);
});

var server = http.listen(3000, function() {
	var host = server.address().address;
	var port = server.address().port;

	console.log('App listening at http://%s%s', host, port);
});
