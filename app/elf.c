#include <stdio.h>
#include "elf.h"

char buffer[512];
FILE *fp;

void read_elf_bytes(int bytes) {
	fread(buffer, bytes, 1, fp);
}

int main() {
	fp = fopen("main.elf", "r");
	read_elf_bytes(0x40);
	struct ElfHeader32 *h = (struct ElfHeader32 *)buffer;
	printf("Entry: %X\n", h->entry);

	return 0;	
}
