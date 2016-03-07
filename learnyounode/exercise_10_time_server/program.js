var net = require('net');
var port = process.argv[2];


function zeroFill(i) {
  return (i < 10 ? '0' : '') + i
}

function now() {
    var date = new Date();
    var year = zeroFill(date.getFullYear());
    var month =  zeroFill(date.getMonth() + 1);     // starts at 0
    var day = zeroFill(date.getDate());      // returns the day of month
    var hours = zeroFill(date.getHours());
    var mins = zeroFill(date.getMinutes());
    var full_date = year + '-' + month + '-' + day + ' ' + hours + ':' + mins;
    return full_date;
}


console.log(now());
var server = net.createServer(function (socket) {
      socket.write(now() + '\n');
      socket.end();
});
server.listen(Number(port));