var express = require('express');
var router = express.Router();

var mongoose = require('mongoose');
var Restaurant = mongoose.model('Restaurant');
var Country = mongoose.model('Country');

var https = require("https");


var isLoggedin = function (req, res, next) {
if (req.isAuthenticated())
    return next();
res.sendfile('views/login.html');
}


module.exports = function(passport){

router.get('/',function(req, res){
    res.sendfile('views/index.html');
});

router.get('/restaurant_page',function(req, res){
    res.sendfile('views/restaurants.html');
});

router.get('/exhibits_page',function(req, res){
    res.sendfile('views/exhibits.html');
});

router.get('/login', function(req, res){
    res.sendfile('views/login.html');
});

/* GET account page/ login page */
router.get('/account_page', isLoggedin, function(req, res){
    res.sendfile('views/account.html');
});

/* Handle Login POST */
router.post('/login', passport.authenticate('login', {
    successRedirect: '/account_page',
    failureRedirect: '/login'
}));

/* GET Registration Page */
router.get('/signup', function(req, res){
    res.sendfile('views/signup.html');
});

/* Handle Registration POST */
router.post('/signup', passport.authenticate('signup', {
    successRedirect: '/',
    failureRedirect: '/signup'
}));

/* Handle Logout */
router.get('/signout', function(req, res) {
    req.logout();
    res.redirect('/');
});

//{loginData: isLoggedin}
router.get('/authenticationData',function(req, res) {
  var isAuth = req.isAuthenticated();
  console.log(req.user);
  if(isAuth) {
    res.json({"loginData": isAuth,
    		  "username": req.user.username,
    		  "_id": req.user._id,
    		  "firstName": req.user.firstName,
    		  "lastName": req.user.lastName});
  }
  else {
    res.json({"loginData": isAuth});
  }
  
});

router.get('/distance', function(req, res, next) {

  var origin = req.query.origin;
  var destination = req.query.destination;
  //console.log(tocity);
  //console.log(fromcity);

  var url = "https://maps.googleapis.com/maps/api/directions/json?origin="+origin+"&destination="+destination+"&key=AIzaSyCYDA_Du0xbuvjKAT-oeWibxO-oqOaDES8";

  var buffer = "", 
              data,
              route;
           var request = https.get(url, function (response) {
          // data is streamed in chunks from the server
          // so we have to handle the "data" event    
          

          response.on("data", function (chunk) {
              buffer += chunk;
          }); 

          response.on("end", function (err) {
              // finished transferring data
              // dump the raw data
              
              //console.log(buffer);
              //console.log("\n");
              data = JSON.parse(buffer);
              route = data.routes[0];
               res.json(data);
              // extract the distance and time
              //console.log("Walking Distance: " + route.legs[0].distance.text);
              //console.log("Time: " + route.legs[0].duration.text);
          }); 
      }); 
     
});

// restaurant routes
router.get('/restaurants', function(req, res, next) {
  Restaurant.find(function(err, restaurants){
    if(err){ return next(err); }
    res.json(restaurants);
  });
});

router.post('/restaurants', function(req, res, next) {
  var restaurant = new Restaurant(req.body);
  restaurant.save(function(err, restaurant){
    if(err){ return next(err); }
    res.json(restaurant);
  });
});

router.param('restaurant', function(req, res, next, id) {
  var query = Restaurant.findById(id);
  query.exec(function (err, restaurant){
    if (err) { return next(err); }
    if (!restaurant) { return next(new Error("can't find restaurant")); }
    req.restaurant = restaurant;
    return next();
  });
});

router.get('/restaurants/:restaurant', function(req, res, next) {
  console.log("found restaurant with id " + req.restaurant._id);
  res.json(req.restaurant);
});

router.get('/restaurantCount', function(req, res, next) {
  Restaurant.find(function(err, restaurants){
    if(err){ return next(err); }
    res.json(restaurants.length);
  });
});


// country routes



router.get('/countries', function(req, res, next) {
  Country.find({}).sort('name').find(function(err, countries){
    if(err){ return next(err); }
    res.json(countries);
  });
});

router.post('/countries', function(req, res, next) {
  var countries = new Country(req.body);
  countries.save(function(err, countries){
    if(err){ return next(err); }
    res.json(countries);
  });
});

router.param('country', function(req, res, next, id) {
  var query = Country.findById(id);
  query.exec(function (err, country){
    if (err) { return next(err); }
    if (!country) { return next(new Error("can't find country")); }
    req.country = country;
    return next();
  });
});

router.get('/countries/:country', function(req, res, next) {
  console.log("found country with id " + req.country._id);
  res.json(req.country);
});

router.get('/countryCount', function(req, res, next) {
  Country.find(function(err, countries){
    if(err){ return next(err); }
    res.json(countries.length);
  });
});



return router;
}