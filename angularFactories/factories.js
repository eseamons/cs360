var app = angular.module('factories', []);

app.factory('geolocation',function() {
    return {
        sayHello: function() {
            return "Hello, World!";
        }
    };
});