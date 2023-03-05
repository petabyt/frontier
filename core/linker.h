#ifndef LINKER_H
#define LINKER_H

struct ElfFileInfo {
	uint32_t max_exec_size;
};

enum LinkerErrors {
	LINK_OK = 0,
	LINK_BAD_FORMAT = 1,
};

int linker_init_elf(void *file, struct ElfFileInfo *info);
uint32_t elf_get_sect_size(void *file, uint32_t of);

#endif
