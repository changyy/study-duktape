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
            console.log('\t\t'+key+'\t:\t'+headers[key]);
        }
    }
}
