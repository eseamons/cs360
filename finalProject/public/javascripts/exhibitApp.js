var app = angular.module('exhibitApp', ['factories']);

app.controller('exhibitCtrl', function($scope,$http,countryFactory,cookieFactory) {
	
	//initialize angular view
	$scope.exhibitResults = [];
	$scope.countries = [];
	$scope.showNoExhibits = false;
	$scope.maxDistance = 25000;
	$scope.countries = countryFactory.getCountriesWithAllOption();
	var origin = cookieFactory.getCookie("origin");
	$scope.currentLocation = origin;
	if(origin != "") {
		$scope.rememberLocation = true;
	}





	$scope.getExhibits = function() {

		alert($scope.exhibitCountry);
	}

}); // end of exhibitCtrl