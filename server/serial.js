var SerialPort = require('serialport');
const Readline = SerialPort.parsers.Readline;

var port = new SerialPort('COM13', {
  baudRate: 9600
});
const parser = port.pipe(new Readline());


module.exports.attachParser = function() {
  parser.on('data', console.log);
}

// Send Time
module.exports.sendTime = function () {
  port.write('Time:'+Math.floor(new Date() / 1000)+'\n');
}

// Send TLE
module.exports.sendTLE = function(TLE) {
  port.write('TLE:'+TLE+'\n');
}