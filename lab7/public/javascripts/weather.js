

$(document).ready(function(){

    $( "#cityfield" ).keyup(function() {
                  var url = "http://ec2-54-218-117-159.us-west-2.compute.amazonaws.com:3000/getcity?q="+$("#cityfield").val().capitalize();
                  $.getJSON(url,function(data) {
                    var everything;
                    everything = "<ul>";
                    $.each(data, function(i,item) {
                      everything += "<li> "+data[i].city+'</li>';
                    });
                    everything += "</ul>";

                    if($("#cityfield").val() !== "") {
                      $("#txtHint").html(everything);
                    }
                    else {
                      $("#txtHint").html('Empty');
                    }
                    
                  })
                  .done(function() { console.log('getJSON request succeeded!'); })
                  .fail(function(jqXHR, textStatus, errorThrown) { 
                    console.log('getJSON request failed! ' + textStatus); 
                    console.log("incoming "+jqXHR.responseText);
                  })
                  .always(function() { console.log('getJSON request ended!');
                  })
                  .complete(function() { console.log("complete"); });
      });

      $("#weather_button").click(function(e){
        var value = $("#cityfield").val();
        value.capitalize();
        $("#cityfield").val("");
        //console.log(value);
        $("#dispcity").text(value);
        var myurl= "http://ec2-54-218-117-159.us-west-2.compute.amazonaws.com:3000/getweather?q="+value;
        //alert(myurl);
        $.ajax({
            url : myurl,
                    dataType : "json",
                    success : function(parsed_json) {
                      console.log(JSON.stringify(parsed_json));
                      var location = parsed_json['location']['city'];
                       var temperature = parsed_json['current_observation']['temperature_string'];
                      var current_weather_forecast = parsed_json['current_observation']['weather'];
                      var weather_report = "<ul>";
                      weather_report += "<li>Location: "+location+"</li>"
                      weather_report += "<li>Temperature: "+temperature+"</li>"
                      weather_report += "<li>Weather: "+current_weather_forecast+"</li>"
                      weather_report += "</ul>";

                      $('#weather').html(weather_report);
                    }
        });
        

        e.preventDefault();
      });

});




