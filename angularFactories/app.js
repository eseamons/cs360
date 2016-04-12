var app = angular.module('app', ['factories']);

app.controller('myCtrl', function($scope, geolocation) {
	$scope.title = "This is a test";
	$scope.hello = geolocation.sayHello();
});