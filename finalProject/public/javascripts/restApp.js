var app = angular.module('restApp', ['factories']);


app.controller('restCtrl', function($scope,$http,countryFactory,cookieFactory) {

	//initialize angular view
	$scope.restResults = [];
	$scope.countries = countryFactory.getCountriesWithAllOption();
	var origin = cookieFactory.getCookie("origin");
	$scope.currentLocation = origin;
	if(origin != "") {
		$scope.rememberLocation = true;
	}
	$scope.showNoRestaurants = false;
	$scope.maxDistance = 25000;

	

    $scope.getRestaurants = function() {

    	if($scope.rememberLocation == true) {
    		origin = cookieFactory.getCookie("origin");
   			cookieFactory.setCookie('origin',$scope.currentLocation,30);
    	}
    	else {
    		cookieFactory.deleteCookie("origin");
    		//alert('the cookie is being deleted');
    	}

	    $http.get('/restaurants').success(function(response) {
	    	$scope.restResults = [];
	    	angular.forEach(response, function(value, key){
	    		 origin = $scope.currentLocation;
	    		 var destination = value.address;
	    		 
	    		 //alert(JSON.stringify(value));
	    		 $http.get('/distance?origin='+origin+'&destination='+destination).success(function(response) {
					var distance = response.routes[0].legs[0].distance.text.replace(" mi","");
					var duration = response.routes[0].legs[0].duration.text.replace(" mins","");
					if(value.country_id == $scope.restCountry && distance <= $scope.maxDistance || $scope.restCountry == -1 && distance <= $scope.maxDistance) {
			         	$scope.restResults.push({_id:value._id,name:value.name,address:value.address,country_id:value.country_id,connections:value.connections,distance:distance,duration:duration});
			        }
					
			    });
	           
	        });
	        /*
	    	alert($scope.restResults.length);
	        if($scope.restResults.length == 0) {
	        	$scope.showNoRestaurants = true;
	        }
	        else {
	        	$scope.showNoRestaurants = false;
	        }
	        */

	        

	    });	

    } // end of getRestaurants method

});