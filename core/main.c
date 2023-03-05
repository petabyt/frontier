#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>

#include <frontier.h>
#include <io.h>
#include <bmp.h>
#include <mjs.h>
#include <elf.h>
#include <linker.h>

#include "../js.h"

void *mdlsym(void *handle, const char *name);
void mem_init();

void *alloc_file(char *filename) {
	struct stat s;
	stat(filename, &s);

	printf("File size: %ld\n", s.st_size);

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
		printf("Coundn't read file");
		return 0;
	}

	fclose(f);

	return buffer;
}

void asm_exec(uintptr_t);

int f_load_app(char *filename) {
	void *buffer = alloc_file(filename);

	if (buffer == NULL) return 1;

	struct ElfFileInfo i;

	int ret = linker_init_elf(buffer, &i);
	if (ret) {
		printf("Linker failure\n");
	} else {
		linker_exec(buffer, &i);
	}

	return 0;
}

int entry() {
	mem_init();

	f_load_app("app/main.elf");

	return 0;
}
