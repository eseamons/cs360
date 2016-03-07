var express = require('express');
var fs = require('fs');
var router = express.Router();
var __dirname = "routes";

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





module.exports = router;
