var fs = require('fs');
var path = require('path');

var directory_path = process.argv[2];
var filter = process.argv[3];

fs.readdir(directory_path,function(err,list) {
    list.forEach(function(file) {
        if(path.extname(file) === "." + filter) {
            console.log(file);
        }
    });   

});S