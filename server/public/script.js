var socket = io.connect();
socket.on('ping-back', function() {
  alert('ping');
});
socket.on('reconnect_failed', function(){
  alert('reconnect failed. please refresh the page.');
});

socket.on('reconnect_error', function(err){
  alert('there was a connection error. Please refresh the page.\n' + err);
});


socket.on('welcome', function(data) {
  // Respond with a message including this clients' id sent from the server
  console.log(data.message);
  var sats = data.message;
  for (i in sats)
  {
    addSatellite(sats[i]);
  }
  socket.emit('i am client', {data: 'foo!', id: data.id});
});

function addSatellite(data){
  var list = document.getElementById('satellitelist');
	var sat = document.createElement('li');
	sat.appendChild(document.createTextNode(data.dispName + " | " + data.noradCatId + " "));
	sat.id = data.id;
	sat.addEventListener("click", clickSat);
	list.insertBefore(sat, list.childNodes[0]);
}

function clickSat(){
  console.log(this.id);
}

function test(){
  socket.emit('i am client', {data: 'hello!'});
  console.log('ping sent');
}

function updateTime(){
  socket.emit('update time', {data: 'update time pls'});
  console.log('requested time update');
}

function updateTLE() {
  socket.emit('update TLE', {data: 'update tle pls'});
  console.log('requested TLE update');
}
