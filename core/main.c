#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <io.h>
#include <bmp.h>
#include <mjs.h>

#include "../js.h"

void *mdlsym(void *handle, const char *name);
void mem_init();

int entry() {
	mem_init();

	sys_init_bmp();

	core_test_js[core_test_js_len - 1] = '\0';
	core_lib_js[core_lib_js_len - 1] = '\0';

	struct mjs *mjs = mjs_create();
	mjs_set_ffi_resolver(mjs, mdlsym);
	mjs_exec(mjs, core_lib_js, NULL);
	mjs_err_t err = mjs_exec(mjs, core_test_js, NULL);
	printf("Result: %s\n", mjs_strerror(mjs, err));
	if (err) {
		sys_exit();
	}

	while (1) {
		msleep(1);
		if (sys_check_key(27)) {
			sys_exit();
		}

		err = mjs_exec(mjs, "renderFrame();", NULL);
		printf("Result: %s\n", mjs_strerror(mjs, err));
	}

	return 0;
}
