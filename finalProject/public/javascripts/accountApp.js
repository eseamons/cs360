var app = angular.module('accountApp', ['factories']);
app.controller('accountCtrl', function($scope,$http,countryFactory) {

	$scope.restaurants = [];
    $scope.countries = countryFactory.getCountries();
    
    $scope.addRestaurant = function() {
    	$scope.newRest = {name:$scope.restName,address:$scope.restAddress, country_id:$scope.restCountry};
    	$scope.create($scope.newRest);
    	$scope.restName = '';
    	$scope.restAddress = '';
        $scope.restCountry = null;
    }

    $scope.create = function(restaurant) {
	    return $http.post('/restaurants', restaurant).success(function(data){
	      $scope.restaurants.push(data);
	    });

    };

    

});