#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>

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

	printf("File size: %d\n", s.st_size);

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

int f_load_app(char *filename) {
	void *buffer = alloc_file(filename);

	struct ElfFileInfo i;

	linker_init_elf(buffer, &i);

	int size = linker_get_exec_size(buffer, &i);
	printf("Calculated size: %d\n", size);
}

int entry() {
	mem_init();

	f_load_app("app/main.elf");
	
	sys_exit();

	return 0;
}
