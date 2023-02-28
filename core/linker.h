struct ElfFileInfo {
	uint32_t max_exec_size;
	uint32_t shstrtab_of;
	uint32_t data_of;
	uint32_t text_of;
	uint32_t got_of;
	uint32_t dynsym_of;
	uint32_t rodata_of;
};

enum LinkerErrors {
	LINK_OK = 0,
	LINK_BAD_FORMAT = 1,
};

int linker_init_elf(void *file, struct ElfFileInfo *info);
uint32_t elf_get_sect_size(void *file, uint32_t of);
