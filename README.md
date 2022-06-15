# Setup

```
% mkdir build
% cd build
% cmake ..
% make
```

# Usage - Test JS Engine

```
% cd build

% ./test-js-engine 
#JS_Engine_Version> 2.7.0
Usage> ./test-js-engine js_file_path

% ./test-js-engine ../resource/test.js 
#JS_Engine_Version> 2.7.0
#JS_Helper_Print>
3
#PASS

% cat ../resource/test.js
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

% ./test-js-engine ../resource/test.js | grep -v "^#"
3
```

# Usage - Test JS Engine with libcurl

```
% cd build
% cat ../resource/url.js 
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

if (typeof _js_helper_fetch_url !== "undefined") {
    //console.log(JSON.stringify(_js_helper_fetch_url("https://ipinfo.io/country")));
    var ret = _js_helper_fetch_url("https://ipinfo.io/country");;
    if (!ret['status']) {
        console.log(JSON.stringify(ret));
    } else {
        console.log('\tstatus:' + ret['status']);
        console.log('\terror_code:' + ret['error_code']);
        console.log('\tresponse_header:\n' + ret['response_header']);
        console.log('\tresponse_body:\n' + ret['response_body']);

        var headers = respsone_header_handler(ret['response_header']);
        for (var key in headers) {
            if (!headers[key] || headers[key].length == 0) {
                console.log('\t\t'+key);
            } else {
                console.log('\t\t'+key+'\t:\t'+headers[key]);
            }
        }
    }
}

% ./test-js-fetch-url-helper ../resource/url.js
#JS_Engine_Version> 2.7.0
#JS_Helper_Fetch_Url>
#JS_Helper_Fetch_Url> input url = [https://ipinfo.io/country]
#JS_Helper_Print>
	status:true
#JS_Helper_Print>
	error_code:0
#JS_Helper_Print>
	response_header:
HTTP/2 200 
access-control-allow-origin: *
x-frame-options: SAMEORIGIN
x-xss-protection: 1; mode=block
x-content-type-options: nosniff
referrer-policy: strict-origin-when-cross-origin
content-type: text/html; charset=utf-8
content-length: 3
date: Wed, 15 Jun 2022 13:07:42 GMT
x-envoy-upstream-service-time: 2
strict-transport-security: max-age=2592000; includeSubDomains
via: 1.1 google
alt-svc: h3=":443"; ma=2592000,h3-29=":443"; ma=2592000


#JS_Helper_Print>
	response_body:
TW

#JS_Helper_Print>
		HTTP/2 200 
#JS_Helper_Print>
		access-control-allow-origin	:	 *
#JS_Helper_Print>
		x-frame-options	:	 SAMEORIGIN
#JS_Helper_Print>
		x-xss-protection	:	 1; mode=block
#JS_Helper_Print>
		x-content-type-options	:	 nosniff
#JS_Helper_Print>
		referrer-policy	:	 strict-origin-when-cross-origin
#JS_Helper_Print>
		content-type	:	 text/html; charset=utf-8
#JS_Helper_Print>
		content-length	:	 3
#JS_Helper_Print>
		date	:	 Wed, 15 Jun 2022 13:07:42 GMT
#JS_Helper_Print>
		x-envoy-upstream-service-time	:	 2
#JS_Helper_Print>
		strict-transport-security	:	 max-age=2592000; includeSubDomains
#JS_Helper_Print>
		via	:	 1.1 google
#JS_Helper_Print>
		alt-svc	:	 h3=":443"; ma=2592000,h3-29=":443"; ma=2592000
#PASS
```
