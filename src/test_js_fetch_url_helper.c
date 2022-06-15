#include <stdio.h>
#include "duktape.h"
#include "duk_v1_compat.h"
#include "my_js_helper_print.h"
#include "my_js_helper_fetch_url.h"

int main(int argc, char *argv[]) {
	printf("#JS_Engine_Version> %ld.%ld.%ld\n", DUK_VERSION/10000 , (DUK_VERSION%10000)/100, DUK_VERSION % 100 );

	if (argc <= 1) {
		printf("Usage> %s js_file_path\n", argv[0]);
		return 0;
	}
	duk_context *ctx = duk_create_heap_default();

	duk_push_global_object(ctx);
	duk_push_c_function(ctx, JS_Helper_Print, 1/*nargs*/);
	duk_put_prop_string(ctx, -2, "_js_helper_print");

	duk_push_c_function(ctx, JS_Helper_Fetch_Url, 1/*nargs*/);
	duk_put_prop_string(ctx, -2, "_js_helper_fetch_url");

	duk_push_c_function(ctx, JS_Helper_Fetch_Url_More, 1/*nargs*/);
	duk_put_prop_string(ctx, -2, "_js_helper_fetch_url_more");

	// https://github.com/svaarala/duktape/blob/master/doc/release-notes-v2-0.rst
	if (duk_peval_file(ctx, argv[1]) != 0) {
		printf("#FAIL, duk_peval_file() error: %s!\n", duk_safe_to_string(ctx, -1));
	} else {
		duk_pop(ctx);
		printf("#PASS\n");
	}
	duk_destroy_heap(ctx);

	return 0;
}
