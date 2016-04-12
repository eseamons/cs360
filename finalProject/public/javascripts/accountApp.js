var app = angular.module('accountApp', ['factories']);
app.controller('accountCtrl', function($scope,$http,countryFactory) {

	$scope.restaurants = [];
    $scope.countries = countryFactory.getCountries();
    
    $http.get('/authenticationData').success(function(data){
        //alert(JSON.stringify(data));
        $scope.account_page_title = 'Account for ' + data.firstName + ' ' + data.lastName;
    });



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