#include "my_js_helper_fetch_url.h"
#include <curl/curl.h>

//
// https://curl.se/libcurl/c/getinmemory.html
//
struct MemoryStruct {
	char *memory;
	size_t size;
};

static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp) {
	size_t realsize = size * nmemb;
	struct MemoryStruct *mem = (struct MemoryStruct *)userp;
	mem->memory = realloc(mem->memory, mem->size + realsize + 1);
	if(mem->memory == NULL) { // out of memory
		printf("#JS_Helper_Fetch_Url> WriteMemoryCallback: out of memory\n");
		return 0;
	}
	memcpy(&(mem->memory[mem->size]), contents, realsize);
	mem->size += realsize;
	mem->memory[mem->size] = 0;
	return realsize;
}

duk_ret_t JS_Helper_Fetch_Url(duk_context *ctx) {
	printf("#JS_Helper_Fetch_Url>\n");
	duk_ret_t ret = -1;
	if (ctx == NULL) {
		return ret;
	}

	char *http_response_body = NULL, *http_response_header = NULL;
	long http_response_code = -1;

	//printf("#JS_Helper_Fetch_Url> input params #: %d\n", duk_get_top_index(ctx));
	const char *url = duk_require_string(ctx, 0);
	printf("#JS_Helper_Fetch_Url> input url = [%s]\n", url);

	//
	// resource allocation
	//
	struct MemoryStruct response_body, response_header;
	response_body.memory = malloc(1);
	response_body.size = 0;
	response_header.memory = malloc(1);
	response_header.size = 0;

	// libcurl Usage
	if (strlen(url) > 0) {
		CURLcode result;
		CURL *curl = curl_easy_init();
		struct curl_slist *chunk = NULL;
		if (curl != NULL) {

			curl_easy_setopt(curl, CURLOPT_URL, url);
			curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
			curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
			curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
			//curl_easy_setopt(curl, CURLOPT_TIMEOUT, 15L);

			curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&response_body);
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
			curl_easy_setopt(curl, CURLOPT_HEADERDATA, (void *)&response_header);
			curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, WriteMemoryCallback);

			result = curl_easy_perform(curl);
			if (result == CURLE_OK) {
				http_response_body = response_body.memory;	
				http_response_header = response_header.memory;
			}

			curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_response_code);
				
			curl_easy_cleanup(curl);
			if (chunk != NULL) {
				curl_slist_free_all(chunk);
			}
		}
	}

	// return object
	//
	// { "status": false, "error_code": 1, "response_code": null, "response_body": null, "response_header": null }
	//
	// https://duktape.org/api.html#duk_push_object
	//
	duk_idx_t obj_idx;
	obj_idx = duk_push_object(ctx);

	int errorCode = 0;

	// { "response_code" : http_response_code }
	if (http_response_code == -1) {
		errorCode = 1;
		duk_push_null(ctx);
	} else {
		duk_push_int(ctx, http_response_code);
	}
	duk_put_prop_string(ctx, obj_idx, "response_code");

	// { "response_body" : http_response_body }
	if (http_response_body == NULL) {
		errorCode = 2;
		duk_push_null(ctx);
	} else {
		duk_push_string(ctx, http_response_body);
	}
	duk_put_prop_string(ctx, obj_idx, "response_body");

	// { "response_header" : http_response_header }
	if (http_response_header == NULL) {
		errorCode = 3;
		duk_push_null(ctx);
	} else {
		duk_push_string(ctx, http_response_header);
	}
	duk_put_prop_string(ctx, obj_idx, "response_header");

	// { "error_code" : errorCode }
	duk_push_int(ctx, errorCode);
	duk_put_prop_string(ctx, obj_idx, "error_code");

	// { "status" : true }
	if (errorCode == 0) {
		duk_push_true(ctx);
	} else {
		duk_push_false(ctx);
	}
	duk_put_prop_string(ctx, obj_idx, "status");

	//duk_pop(ctx);

	ret = 1;

	//
	// resource release
	//
	free(response_body.memory);
	free(response_header.memory);

	return ret;
}
