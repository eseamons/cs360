$(document).ready(function(){
    $("#serialize").click(function(){
        var myobj = {Name:$("#Name").val(), URL:$("#ImageURL").val(), Comment:$("#Comment").val()};
        jobj = JSON.stringify(myobj);
        $("#json").text(jobj);
        
        var url = "comment";
		$.ajax({
		  url:url,
		  type: "POST",
		  data: jobj,
		  contentType: "application/json; charset=utf-8",
		  success: function(data,textStatus) {
		      //$("#done").html(textStatus);
		  }
		}) // end of ajax call

    }); // end of serialize click event


    $("#getThem").click(function() {
      $.getJSON('comment', function(data) {
        var everything = "";
        for(var comment in data) {
          com = data[comment];

        
          everything += '<div class="row">';
          everything += '<div class="col-sm-3">';
          everything += '<div class="well">';
          everything += '<span>'+ com.Name +'</span>';
          everything += '<img src="'+com.URL+'" style="margin-left:30px;" class="img-circle" height="40" width="40" alt="Avatar">';
          everything += '</div>';
          everything += '</div>';
          everything += '<div class="col-sm-9">';
          everything += '<div class="well">';
          everything +=  '<p>' + com.Comment + '</p>';
          everything +=  '</div>';
          everything +=    '</div>';
          everything += '</div>';


          //everything += "<li>Name: " + com.Name + " -- Comment: " + com.Comment + "</li>";
        }
        //everything += "</ul>";
        $("#comments").html(everything);
      })
    }) // end of getThem click event


});// end of document ready event