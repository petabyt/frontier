#ifndef LINKER_H
#define LINKER_H

extern char *linker_error1;
extern char *linker_error2;

struct ElfFileInfo {
	uint32_t max_exec_size;
	uint32_t strtab_of;
	uint32_t symtab_of;
};

enum LinkerErrors {
	LINK_OK = 0,
	LINK_BAD_FORMAT = 1,
};

int linker_init_elf(void *file, struct ElfFileInfo *info);
uint32_t elf_get_sect_size(void *file, uint32_t of);
struct ElfSectHeader32 *get_elf_head(void *file, uint32_t i);
char *elf_head_name(void *file, uint32_t i);
uintptr_t linker_get_symbol(void *file, struct ElfFileInfo *info, char *name);
uint32_t linker_exec(void *file, struct ElfFileInfo *info);

#endif
