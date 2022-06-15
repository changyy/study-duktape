if (typeof console === "undefined") {
	console = {}
}
if (typeof console.log === "undefined") {
	console.log = function (input) {
		if (typeof _js_helper_print !== "undefined") {
			_js_helper_print(""+input);
		}
	}
}

var a = 1;
var b = 2;
var c = a+b;

console.log(c);
