#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <io.h>
#include <bmp.h>
#include <mjs.h>

#include "../js.h"

void mem_init();

void foo() {
	printf("Testing\n");
}

void *mdlsym(void *handle, const char *name) {
	if (!strcmp(name, "uart_str")) return uart_str;
	if (!strcmp(name, "font_print_string")) return font_print_string;
	if (!strcmp(name, "bmp_apply")) return bmp_apply;
	return NULL;
}

int entry() {
	mem_init();

	sys_init_bmp();

	bmp_clear(0x444444);
	bmp_apply();

	core_test_js[core_test_js_len - 1] = '\0';

	struct mjs *mjs = mjs_create();
	mjs_set_ffi_resolver(mjs, mdlsym);
	mjs_err_t err = mjs_exec(mjs, core_test_js, NULL);
	printf("Result: %s\n", mjs_strerror(mjs, err));

	msleep(10000);

	sys_exit();
	return 0;
}
