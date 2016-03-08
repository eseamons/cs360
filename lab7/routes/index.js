var express = require('express');
var fs = require('fs');
var router = express.Router();
var __dirname = "routes";

var request = require('request');


/* GET home page. */
router.get('/', function(req, res) {
  res.sendFile('weather.html', { root:  'public' });
});// end of / route

router.get('/test1.html', function(req, res) {
  res.sendFile('test1.html', { root:  'public' });
});// end of /test1.html route

router.get('/test2.txt', function(req, res) {
  res.sendFile('test2.txt', { root:  'public' });
});// end of /test2.txt route

router.get('/test3.gif', function(req, res) {
  res.sendFile('test3.gif', { root:  'public' });
});// end of /test3.gif route

router.get('/test4.jpg', function(req, res) {
  res.sendFile('test4.jpg', { root:  'public' });
});// end of /test4.jpg route

router.get('/getcity',function(req,res,next) {
     console.log("In getcity route");

     fs.readFile(__dirname + '/cities.dat.txt',function(err,data) {
            if(err) throw err;
            var cities = data.toString().split("\n");
            var query_string = req.query.q;
            if(query_string === undefined) {
            	query_string = "";
            }
            var myRe = new RegExp("^" + query_string);


            var jsonresult = [];

            

			    for(var i = 0; i < cities.length; i++) {
			      var result = cities[i].search(myRe); 
			      if(result != -1) {
			       console.log(cities[i]);
			       jsonresult.push({city:cities[i]});
			      } 
			    }  

		    console.log(jsonresult);


		    res.status(200).json(jsonresult);


      })




}); // end of /getcity route




router.get('/getweather',function(req,res,next) {
    console.log("In getweather route");
    var query_city = req.query.q;
    console.log("get weather for " + query_city);

    var myurl= "http://api.wunderground.com/api/db641ad49bb24da6/geolookup/conditions/q/UT/";
    myurl += query_city;
    myurl += ".json";


    request(myurl, function (error, response, body) {
        res.status(response.statusCode).json(JSON.parse(body));
    })


}); // end of /getweather route



router.get('/getmovieinfo',function(req,res,next) {
    console.log("In getmovieinfo route");
    var query_movie = req.query.q;
    console.log("get movie info for " + query_movie);

    var myurl= "http://api.themoviedb.org/3/search/movie?query="+query_movie+"&api_key=2428d1c265f0b030719a14e20f3c086a";


    request(myurl, function (error, response, body) {
        res.status(response.statusCode).json(JSON.parse(body));
    })


}); // end of /getweather route





module.exports = router;
