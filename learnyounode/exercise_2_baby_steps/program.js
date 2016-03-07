var array = process.argv.splice(2);
var i;
var sum = 0;

for(i = 0; i < array.length; i++) {
	sum += Number(array[i]);
}

console.log(sum);