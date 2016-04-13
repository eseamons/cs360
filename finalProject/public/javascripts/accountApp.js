var app = angular.module('accountApp', ['factories']);
app.controller('accountCtrl', function($scope,$http,countryFactory) {

	$scope.restaurants = [];
    $scope.countries = countryFactory.getCountries();
    $scope.data = {};
    $scope.data.countries = $scope.countries;
    $scope.data.accountName = '';
    $scope.data.accountEmail = '';
    $scope.showModal = false;
    $scope.showInfoModal = false;
    $scope.authData = '';

    $http.get('/authenticationData').success(function(results){
        $scope.authData = results;
        $scope.account_page_title = 'Account for ' + results.firstName + ' ' + results.lastName;
        $scope.data.accountName = results.firstName + ' ' + results.lastName;
        $scope.data.accountEmail = results.email;
        $scope.data.accountUsername = results.username;
    });

    $scope.toggleAddRestModal = function() {

        $scope.data.restCountry = null;
        $scope.data.restName = '';
        $scope.data.restAddress = '';
        $scope.showModal = true;

    }

    $scope.toggleAccountInfoModal = function() {
        $scope.showInfoModal = true;
    }

    $scope.addRestaurant = function() {
        //alert($scope.data.restCountry);
        //alert($scope.data.restName);
        //alert($scope.data.restAddress);

    	$scope.newRest = {name:$scope.data.restName,address:$scope.data.restAddress, country_id:$scope.data.restCountry};
    	$scope.create($scope.newRest);

        $scope.data.restName = '';
        $scope.data.restAddress = '';
        $scope.data.restCountry = null;
        $scope.showModal = false;
    }

    $scope.create = function(restaurant) {
	    return $http.post('/restaurants', restaurant).success(function(data){
	      $scope.restaurants.push(data);
	    });

    };

    

});