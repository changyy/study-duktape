#include "my_js_helper_print.h"

duk_ret_t JS_Helper_Print(duk_context *ctx) {
	printf("#JS_Helper_Print>\n");
	duk_ret_t ret = -1;
	if (ctx != NULL) {
		const char *message = duk_require_string(ctx, 0);
		printf("%s\n", message);
		ret = 0;
	}
	return ret;
}
