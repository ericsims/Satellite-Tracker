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

var selectedSatellite = -1;
function clickSat(){
  console.log(this.id);
  if(selectedSatellite == this.id) {
    deselectSat(this);
  } else {
    selectSat(this);
  }
}

function selectSat(element){
  if(selectedSatellite != -1){
    deselectSat(document.getElementById(selectedSatellite));
  }
  element.classList.add("selected");
  selectedSatellite = element.id;
  socket.emit('switch satellite', {data: element.id});
  console.log('requested Satellite changes');
}

function deselectSat(element){
  if(selectedSatellite != -1){
    element.classList.remove("selected");
  }
  selectedSatellite = -1;
  console.log('deselect');
}

function test(){
  socket.emit('i am client', {data: 'hello!'});
  console.log('ping sent');
}

function updateTime(){
  socket.emit('update time', {data: 'update time pls'});
  console.log('requested time update');
}

function switchSat(){
  socket.emit('switch satellite', {data: 'ISS'});
  console.log('requested Satellite changes');
}

function updateTLE() {
  socket.emit('update TLE', {data: 'update tle pls'});
  console.log('requested TLE update');
}
