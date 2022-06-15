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

function respsone_header_handler(input) {
	var output = [];
	var lines = input.split("\r\n");
	for(var i=0, cnt=lines.length ; i<cnt ; ++i) {
		if (lines[i].length == 0)
			continue;
		var line = lines[i].split(":");
		var key = line.shift();
		output[ key ] = line.join(":");
	}
	return output;
}

if (typeof _js_helper_fetch_url_more !== "undefined") {
	var ret = _js_helper_fetch_url_more({
        url: 'https://ipinfo.io/country',
        request_header: [
            'Content-Type: HelloWorld',
            'X-MAN: HelloWorld',
        ],
    });
	console.log(JSON.stringify(ret));
}
