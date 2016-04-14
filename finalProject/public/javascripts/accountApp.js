var app = angular.module('accountApp', ['factories']);
app.controller('accountCtrl', function($scope,$http,countryFactory) {

	$scope.restaurants = [];
    $scope.countries = countryFactory.getCountries();
    $scope.data = {};
    $scope.data.countries = $scope.countries;
    $scope.data.accountName = '';
    $scope.data.accountEmail = '';
    $scope.data.myRestList = [];
    $scope.showModal = false;
    $scope.showInfoModal = false;
    $scope.showMyRestModal = false;
    $scope.authData = '';


    $http.get('/authenticationData').success(function(results){
        $scope.authData = results;
        $scope.account_page_title = 'Account for ' + results.firstName + ' ' + results.lastName;
        $scope.data.accountName = results.firstName + ' ' + results.lastName;
        $scope.data.accountEmail = results.email;
        $scope.data.accountUsername = results.username;
        $http.get('/user_added_restaurants'+'?user_id='+$scope.authData._id).success(function(results) {
    		angular.forEach(results, function(value, key){
            	$scope.data.myRestList.push({name: value.name, country_name: countryFactory.getCountryNameById(value._id)});
            });
            //$scope.data.myRestList = results;
        }); 
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

    $scope.toggleViewMyRestModal = function() {
    	$scope.data.myRestList = [];
    	$http.get('/user_added_restaurants'+'?user_id='+$scope.authData._id).success(function(results) {
    		angular.forEach(results, function(value, key){
    			var country_name = countryFactory.getCountryNameById(value.country_id,$scope.data.countries);
            	$scope.data.myRestList.push({name: value.name, country_name: country_name, address: value.address, connections: value.connections});
            });
            //$scope.data.myRestList = results;
        });
        
    	$scope.showMyRestModal = true;
    }

    $scope.addRestaurant = function() {
        //alert($scope.data.restCountry);
        //alert($scope.data.restName);
        //alert($scope.data.restAddress);

    	$scope.newRest = {name:$scope.data.restName,address:$scope.data.restAddress, country_id:$scope.data.restCountry, user_id:$scope.authData._id};
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