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
uintptr_t linker_get_symbol(void *file, struct ElfFileInfo *info, char *name);
uint32_t linker_exec(void *file, struct ElfFileInfo *info);
uintptr_t linker_scan_symbols(void *file, struct ElfFileInfo *info);

// Get Magic Lantern compatible symbol
void *ml_sym(char *name);

#endif
