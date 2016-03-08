$(document).ready(function(){

			$("#movie_search_button").click(function(e){
				var movie_name = $("#movie_name_field").val();
				$("#movie_name_field").val("");
				
				var movie_search_url = "http://ec2-54-218-117-159.us-west-2.compute.amazonaws.com:3000/getmovieinfo?q="+movie_name;

				$.ajax({
			  		url : movie_search_url,
                    dataType : "json",
                    success : function(parsed_json) {

                    	if(parsed_json['total_results'] != 0) {
                    		var movie_title = parsed_json['results'][0]['title'];
                    		var movie_summary = parsed_json['results'][0]['overview'];
                    		var movie_poster = parsed_json ['results'][0]['poster_path'];
                    		movie_poster_url = "http://image.tmdb.org/t/p/w300/"+movie_poster;
	                    	$('#movie_info_title').text(movie_title);

	                    	$('#movie_poster').html('');
	                    	if($('#include_movie_poster').is(":checked") === true) {
	                    		$('#movie_poster').html('<img id="movie_poster_image" src="'+movie_poster_url+'"">');
	                    	}
	                    		
	                    	$('#movie_info_summary').text(movie_summary);
	                    	//alert(movie_poster);
	                    	//alert(JSON.stringify(movie_summary));
                    	}
                    	else {
                    		$('#movie_info_title').text(" Movie Not Found");
                    		$('#movie_poster').html('');
                    		$('#movie_info_summary').text('');
                    	}
                    	// example image
                    	//http://image.tmdb.org/t/p/w300/6u1fYtxG5eqjhtCPDx04pJphQRW.jpg
                    }
			  	});



				e.preventDefault();
			});

});