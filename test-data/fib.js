
var nCalls = 0;

function fib(n) {
	if (n < 2) return n;
	return fib(n-1) + fib(n-2);
}

var n = 10;
var f_n = fib(n);
console.log("fib(10)");
console.log(f_n);
console.log("nCalls");
console.log(nCalls);
