#include <stdlib.h>
#include <elf.h>
#include <app.h>
#include <linker.h>

int load_app_metadata(FILE *f) {
	return 0;
}

int load_app_elf(FILE *f) {
	struct ElfFileInfo i;

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
