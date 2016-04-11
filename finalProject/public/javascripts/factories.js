var app = angular.module('factories', []);

app.factory('countryFactory',function($http) {
    return {
        getCountriesWithAllOption: function() {
            var countries = [];
            $http.get('/countries').success(function(response) {
		    	countries.push({"name": "All", "_id": -1});
		    	angular.forEach(response, function(value, key){
		    		countries.push(value);
		    	});

		    });
            return countries;
        },
        getCountries: function() {
        	var countries = [];
        	$http.get('/countries').success(function(response) {
		    	angular.copy(response,countries)
		    });
		    return countries;
        }
    };
});

app.factory('cookieFactory',function($http) {
    return {
        setCookie: function(cname,cvalue,exdays) {
            var d = new Date();
            d.setTime(d.getTime() + (exdays*24*60*60*1000));
            var expires = "expires=" + d.toGMTString();
            document.cookie = cname+"="+cvalue+"; "+expires;

        },
        getCookie: function(cname) {
            var name = cname + "=";
            var ca = document.cookie.split(';');
            for(var i=0; i<ca.length; i++) {
                var c = ca[i];
                while (c.charAt(0)==' ') c = c.substring(1);
                if (c.indexOf(name) == 0) {
                    return c.substring(name.length, c.length);
                }
            }
            return "";
        },
        deleteCookie: function(cname) {
            document.cookie = cname+"=; expires=Thu, 01 Jan 1970 00:00:00 UTC";
        }
    };
});

// https://maps.googleapis.com/maps/api/directions/json?origin=Provo&destination=Orem&key=AIzaSyCYDA_Du0xbuvjKAT-oeWibxO-oqOaDES8