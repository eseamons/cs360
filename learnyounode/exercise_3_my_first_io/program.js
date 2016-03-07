var fs = require('fs');
var path = process.argv[2];
var buffer = fs.readFileSync(path);
var buf_contents = buffer.toString();
var str_array = buf_contents.split('\n');
var num_lines = str_array.length - 1;
console.log(num_lines);
