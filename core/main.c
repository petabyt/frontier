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

	mjs_exec(mjs, "renderFrame();", NULL);

	int last_mouse = 0;
	int render_sleeps = 0;
	while (1) {
		last_mouse = sys_check_mouse();
		if (sys_check_key(27)) {
			sys_exit();
		}

		if (last_mouse) {
			//printf("Down\n");
		}

		msleep(1);
		render_sleeps++;
		if (last_mouse == 1 && sys_check_mouse() == 0) {
			printf("Clicked\n");
			mjs_exec(mjs, "clicks++;", NULL);
			mjs_exec(mjs, "renderFrame();", NULL);
		}

		if (render_sleeps > 500) {
			render_sleeps = 0;
			mjs_exec(mjs, "renderFrame();", NULL);
		}
	}

	sys_exit();
	return 0;
}
