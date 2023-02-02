#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

int main() {
	FILE *f = fopen("symtbl.bin", "w");

	char buffer[256];
	FILE *c = popen("arm-none-eabi-readelf -s os.elf", "r");
	if (c == NULL) {
		return 1;
	}

	while (fgets(buffer, sizeof(buffer), c) != NULL) {
		int num, value, size;
		char type[128], bind[128], vis[128], ndx[64], name[128];
		int r = sscanf(buffer, "%d: %x %d %s %s %s %s %s", &num, &value, &size, type, bind, vis, ndx, name);
		if (r == EOF) {
			continue;
		}

		if (!strcmp(type, "FUNC")) {
			char entry[256];
			uint32_t addr = value;
			memcpy(entry, &addr, 4);
			uint32_t length = strlen(name) + 1;
			memcpy(entry + 4, &length, 4);
			memcpy(entry + 8, name, length);
			fwrite(entry, 1, 4 + 4 + length, f);
		}
	}

	fclose(f);
}
