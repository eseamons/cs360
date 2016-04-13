var app = angular.module('accountApp', ['factories']);
app.controller('accountCtrl', function($scope,$http,countryFactory) {

	$scope.restaurants = [];
    $scope.countries = countryFactory.getCountries();
    $scope.data = {};
    $scope.data.countries = $scope.countries;
    $scope.showModal = false;
    $scope.authData = '';

    $http.get('/authenticationData').success(function(data){
        $scope.authData = data;
        $scope.account_page_title = 'Account for ' + data.firstName + ' ' + data.lastName;
    });

    $scope.toggleModal = function() {

        $scope.data.restCountry = null;
        $scope.data.restName = '';
        $scope.data.restAddress = '';
        $scope.showModal = true;

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