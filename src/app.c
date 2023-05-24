#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>

#include <sys.h>
#include <elf.h>
#include <app.h>
#include <linker.h>

int parse_app_metadata(FILE *f) {
	return 0;
}

int load_app_elf(FILE *f) {
	//struct ElfFileInfo i;
	return 0;
}

int load_app(char *filename) {
	FILE *f = fopen(filename, "rb");
	if (f == NULL) {
		return -1;
	}

	char tmp[128];
	fread(tmp, 1, sizeof(tmp), f);

	struct AppMetaData *md = (struct AppMetaData *)tmp;
	if (md->magic1 == 'F' && md->magic2 == 'R') {
		return parse_app_metadata(f);
	}

	uint32_t *chk32 = (uint32_t *)tmp;
	if (chk32[0] == ELF_MAGIC) {
		return load_app_elf(f);
	}

	return -2;
}

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

	// ...
	
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
