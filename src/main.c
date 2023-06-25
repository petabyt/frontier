#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>

#include <sys.h>
#include <bmp.h>
#include <ui.h>

int main_menu() {
	ui_text("FrontierOS software demo", 0xffffff);

	if (ui_button("Render an image")) {
		extern char settings_bmp[];
		bmp_render_bmp(settings_bmp, 200, 200);
	}

	if (ui_button("Load ELF module")) {
		sys_load_app("app/tetris/tetris.elf");
	}

	return 0;
}

int ui_demo_loop() {
	ui_reset();
	sys_init_bmp();

	bmp_clear(0x0);

	while (1) {
		if (ui_frame(main_menu)) {
			return 0;
		}

		msleep(50);
	}	

}

int entry() {
	sys_init_mem();
	sys_init_syms();
	log_cpu_info();

	sys_load_app("app/js/js.elf");

	uint8_t *s = sym("mjs_ffi_call");
	sys_debug("Test sym for push inst: %02X %02X %02X %02X\n", s[0], s[1], s[2], s[3]);

	return 0;
}
