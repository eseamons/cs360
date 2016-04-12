$(document).ready(function() {
    //alert("test");
    $.ajax({
			  		url : '/authenticationData',
                    dataType : "json",
                    success : function(result) {

                    	if(result.loginData == false) {
                    		$( ".navbar-link-options" ).append('<li><a href="/login">Log in</a></li>');
                    	}
                    	else {
                            $( ".navbar-link-options" ).append('<li><a href="/account_page">My Account</a></li>');
                    		$( ".navbar-link-options" ).append('<li><a href="/signout">Sign Out</a></li>');
                    		$(".navbar-link-options").append('<p class="navbar-text">Signed in as '+ result.firstName + ' ' + result.lastName +'</p>');
                    	}
                    }
                    
	});


});