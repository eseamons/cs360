var http = require('http');
//var https = require('https');

var url = process.argv[2];
//var url = "https://maps.googleapis.com/maps/api/directions/json?origin=Provo&destination=Orem&key=AIzaSyCYDA_Du0xbuvjKAT-oeWibxO-oqOaDES8";

http.get(url, function(response) {
   response.setEncoding('utf8');
   response.on('data', console.log);
   response.on('error', console.error);
});