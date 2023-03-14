#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

// Include JSON parser
#include "mjs/src/frozen/frozen.c"

#pragma pack(push, 1)

struct AppMetaData {
	uint32_t api_version;
	uint32_t header_size;
	char name[20];
	char author[20];
	char url[40];
	uint8_t icon[];
};

struct TblEntry {
	uint32_t addr;
	uint16_t length;
	char string[];
};

#pragma pack(pop)

char *readelf = "arm-none-eabi-readelf";

int add_syms(char *input, char *output) {
	FILE *f = fopen(output, "rwb+");
	if (f == NULL) {
		puts("Error opening file");
		return 1;
	}

	fseek(f, 0, SEEK_END);

	char buffer[256];
	sprintf(buffer, "%s -s %s", readelf, input);
	FILE *c = popen(buffer, "r");
	if (c == NULL) {
		return 1;
	}

	while (fgets(buffer, sizeof(buffer), c) != NULL) {
		int num, value, size;
		char type[128], bind[128], vis[128], ndx[64], name[128] = {0};

		int r = sscanf(buffer, "%d: %x %d %s %s %s %s %s", &num, &value, &size, type, bind, vis, ndx, name);
		if (r == EOF) {
			continue;
		}

		if (!strcmp(type, "FUNC")) {
			struct TblEntry e;
			e.addr = value;
			e.length = strlen(name);
			memcpy(e.string, name, e.length);
			fwrite(&e, 1, 4 + 4 + e.length, f);
		}
	}

	fclose(f);
	fclose(c);

	return 0;
}

int gen_app_meta(struct AppMetaData *amd, char *content) {
	char *name;
	char *url;
	char *author;
	char *icon;
	int version;

	json_scanf(content, strlen(content), "{name: %Q, url: %Q, author: %Q, version: %B, icon: %Q}",
		&name, &url, &author, &version, &icon);

	strcpy(amd->name, name);
	strcpy(amd->url, author);
	strcpy(amd->author, author);
	amd->header_size = 0;
	amd->api_version = 0;

	printf("Name: %s\n", name);
}

int main(int argc, char *argv[]) {
	char *input = "os.elf";
	char *output = "os.bin";
	for (int i = 0; i < argc; i++) {
		if (argv[i][0] != '-') continue;
		switch (argv[i][1]) {
		case 'i':
			input = argv[i + 1];
			break;
		case 'o':
			output = argv[i + 1];
			break;
		case 's':
			// Append single row of symbols
			return add_syms(input, output);
		case 'a': {
			char *content = json_fread(argv[i + 1]);
			struct AppMetaData amd;
			gen_app_meta(&amd, content);
		}
		}
	}

	puts("No valid args");
	return 0;
}
