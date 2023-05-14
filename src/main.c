#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>

#include <sys.h>
#include <bmp.h>
#include <linker.h>
#include <sym.h>
#include <ui.h>

void *alloc_file(char *filename) {
	struct stat s;
	stat(filename, &s);

	printf("File size: %lu\n", s.st_size);

	void *buffer = malloc(s.st_size);
	if (buffer == NULL) {
		printf("Failed to allocate ELF buffer\n");
		return 0;
	}

	FILE *f = fopen(filename, "rb");
	if (f == NULL) {
		printf("Failed to open file\n");
		return 0;
	}

	size_t x = fread(buffer, 1, 64, f);
	x += fread(buffer + 64, 1, s.st_size - 64, f);
	if (x != s.st_size) {
		printf("Coundn't read file\n");
		return 0;
	}

	fclose(f);

	return buffer;
}

int sys_load_app(char *filename) {
	void *buffer = alloc_file(filename);

	struct stat s;
	stat(filename, &s);
	sys_segment(buffer, s.st_size);

	if (buffer == NULL) return 1;

	struct ElfFileInfo i;

	printf("Linking @ %X\n", buffer);

	int ret = linker_init_elf(buffer, &i);
	if (linker_error1 != NULL) {
		if (linker_error2 == NULL) {
			printf("ELF: %s\n", linker_error1);
		} else {
			printf("ELF: %s%s\n", linker_error1, linker_error2);
		}
	}

	linker_scan_symbols(buffer, &i);

	if (!ret) {
		printf("return: %lX\n", linker_exec(buffer, &i));
	}

	return 0;
}

int main_menu() {
	ui_text("FrontierOS / Fujihack software demo", 0xffffff);

	if (ui_button("Render an image")) {
		extern char settings_bmp[];
		bmp_render_bmp(settings_bmp, 200, 200);
	}

	if (ui_button("Load ELF module")) {
		sys_load_app("app/tetris/tetris.elf");
	}

	return 0;
}

int entry() {
	sys_init_syms();
	sys_init_mem();

	ui_reset();
	sys_init_bmp();

	bmp_clear(0x0);

	sys_load_app("app/tetris/tetris.elf");

return 1;
	while (1) {
		if (ui_frame(main_menu)) {
			return 0;
		}

		msleep(50);
	}	

	return 0;
}
