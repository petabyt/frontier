#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>

#include <sys.h>
#include <bmp.h>
#include <linker.h>
#include <buttons.h>
#include <js.h>
#include <sym.h>

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

	size_t x = fread(buffer, 1, s.st_size, f);
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

	int ret = linker_init_elf(buffer, &i);
	if (linker_error1 != NULL) {
		if (linker_error2 == NULL) {
			printf("ELF: %s\n", linker_error1);
		} else {
			printf("ELF: %s%s\n", linker_error1, linker_error2);
		}
	}

	if (!ret) {
		printf("return: %d\n", linker_exec(buffer, &i));
	}

	return 0;
}

//int strcasecmp(const char *a, const char *b) { return strcasecmp(a, b); }
//int strncasecmp(const char *a, const char *b, size_t c) { return strncasecmp(a, b, c); }

int uart_welcome();
void uart_prompt();

int entry() {
	sys_init_syms();
	sys_init_mem();
	js_init();

	sys_load_app("app/hello/hello.elf");

	return 0;
}
