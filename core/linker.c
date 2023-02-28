// Frontier ELF Linker

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <elf.h>
#include <linker.h>

int linker_init_elf(void *file, struct ElfFileInfo *info) {
	memset(info, 0, sizeof(struct ElfFileInfo));

	struct ElfHeader32 *h = (struct ElfHeader32 *)file;

	if (h->magic != ELF_MAGIC) {
		return LINK_BAD_FORMAT;
	}
	
	printf("Entry: %X\n", h->phoff);
	printf("Entries: %d\n", h->phnum);

	// Read program table header entries
	for (int i = 0; i < h->phnum; i++) {
		struct ElfProgHeader32 *p = (struct ElfProgHeader32 *)
			(file + h->phoff + (i * h->phentsize));
		printf("PH Entry type: %X\n", p->type);
	}

	printf("---\n");

	struct ElfSectHeader32 *names = (struct ElfSectHeader32 *)
				(file + h->shoff + (h->shstrndx * h->shentsize));
	printf("Names offset %X\n", names->offset);

	for (int i = 0; i < h->shnum; i++) {
		struct ElfSectHeader32 *s = (struct ElfSectHeader32 *)
			(file + h->shoff + (i * h->shentsize));

		char *sect_name = (char *)file + names->offset + s->name;

		printf("SH Entry name: %s\n", sect_name);
		printf("SH Entry type: %X\n", s->type);
		printf("SH Entry offset: %X\n", s->offset);

		uint32_t of = h->shoff + (i * h->shentsize);

		if (!strcmp(sect_name, ".text")) {
			info->text_of = of;
		} else if (!strcmp(sect_name, ".data")) {
			info->data_of = of;
		} else if (!strcmp(sect_name, ".dynsym")) {
			info->dynsym_of = of;
		} else if (!strcmp(sect_name, ".rodata")) {
			info->rodata_of = of;
		}
	}

	return 0;
}

uint32_t elf_get_sect_size(void *file, uint32_t of) {
	struct ElfSectHeader32 *s = (struct ElfSectHeader32 *)(file + of);
	return s->size;
}

uint32_t linker_get_exec_size(void *file, struct ElfFileInfo *info) {
	return elf_get_sect_size(file, info->text_of) + elf_get_sect_size(file, info->data_of)
		+ elf_get_sect_size(file, info->rodata_of);
}

uint32_t linker_map(void *file, struct ElfFileInfo *info, void *output) {
	
}
