// Frontier ELF Linker

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <elf.h>
#include <linker.h>

char *linker_ok = "Successfully linked";
char *linker_undefined_sym = "Couldn't find symbol: ";
char *linker_incompatible = "Binary is incompatible with this sytem";
char *linker_broken = "File is broken";

char *linker_error1 = NULL;
char *linker_error2 = NULL;

struct ElfSectHeader32 *get_elf_head(void *file, uint32_t i) {
	struct ElfHeader32 *h = (struct ElfHeader32 *)file;
	return (struct ElfSectHeader32 *)(file + h->shoff + (i * h->shentsize));
}

char *elf_head_name(void *file, uint32_t i) {
	struct ElfHeader32 *h = (struct ElfHeader32 *)file;
	struct ElfSectHeader32 *names = (struct ElfSectHeader32 *)
		(file + h->shoff + (h->shstrndx * h->shentsize));
	return (char *)file + names->offset + i;
}

int linker_relocate(void *file, struct ElfFileInfo *info, struct ElfSectHeader32 *s) {
	// To relocation list (.rel.text)
	struct ElfSectHeader32 *l = get_elf_head(file, s->link);

	// To target (text)
	struct ElfSectHeader32 *target = get_elf_head(file, s->info);

	void *base = file + target->offset;

	// To symtab
	struct ElfSym32 *syms = (struct ElfSym32 *)(file + l->offset);

	// To strtab
	struct ElfSectHeader32 *str = get_elf_head(file, l->link);

	// Perform the relocations
	struct ElfRel32 *relocs = (struct ElfRel32 *)(file + s->offset);
	for (int i = 0; i < s->size / s->entsize; i++) {
		int index = relocs[i].info >> 8;
		int type = (uint8_t)(relocs[i].info);

		char *name = (char *)(file + str->offset + syms[index].name);

		uint32_t *target_loc = (void *)(file + target->offset + relocs[i].offset);
		if (type == R_ARM_CALL) {
			if (syms[index].value == 0 && syms[index].shndx == 0) {
				void *call = f_dlsym(NULL, name);
				if (call == NULL) {
					printf("Undefined external function %s\n", name);
					linker_error1 = linker_undefined_sym;
					linker_error2 = name;
					return 1;
				}
				asm_gen_call(target_loc, call);
			} else {
				asm_gen_call(target_loc, file + syms[index].value + get_elf_head(file, syms[index].shndx)->offset);
			}
		} else if (type == R_ARM_ABS32) {
			target_loc[0] = file + target_loc[0] + syms[index].value + get_elf_head(file, syms[index].shndx)->offset;
		} else if (type == R_ARM_REL32) {
			// Confusing pointer order of operations
			target_loc[0] += (get_elf_head(file, syms[index].shndx)->offset) - (target->offset + relocs[i].offset);
		}
	}

	return 0;
}

int linker_init_elf(void *file, struct ElfFileInfo *info) {
	memset(info, 0, sizeof(struct ElfFileInfo));

	struct ElfHeader32 *h = (struct ElfHeader32 *)file;

	if (h->magic != ELF_MAGIC) {
		linker_error1 = linker_broken;
		linker_error2 = NULL;
		return 1;
	}
	
	// Read program table header entries
	for (int i = 0; i < h->phnum; i++) {
		struct ElfProgHeader32 *p = (struct ElfProgHeader32 *)
			(file + h->phoff + (i * h->phentsize));
	}

	struct ElfSectHeader32 *names = (struct ElfSectHeader32 *)
				(file + h->shoff + (h->shstrndx * h->shentsize));

	for (int i = 0; i < h->shnum; i++) {
		struct ElfSectHeader32 *s = (struct ElfSectHeader32 *)
			(file + h->shoff + (i * h->shentsize));

		char *sect_name = (char *)file + names->offset + s->name;

		if (s->link != 0) {
			struct ElfSectHeader32 *s2 = (struct ElfSectHeader32 *)
				(file + h->shoff + (s->link * h->shentsize));
		}

		if (s->info != 0) {
			struct ElfSectHeader32 *s2 = (struct ElfSectHeader32 *)
				(file + h->shoff + (s->info * h->shentsize));
		}

		if (s->type == SHT_PROGBITS) {
			info->max_exec_size += s->size;
		} else if (s->type == SHT_REL) {
			if (linker_relocate(file, info, s)) {
				return 1;
			}
		}

		uint32_t of = h->shoff + (i * h->shentsize);

		if (!strcmp(sect_name, ".symtab")) {
			info->symtab_of = of;
		} else if (!strcmp(sect_name, ".strtab")) {
			info->strtab_of = of;
		}
	}

	linker_error1 = linker_ok;
	linker_error2 = NULL;

	return 0;
}

uintptr_t linker_get_symbol(void *file, struct ElfFileInfo *info, char *name) {
	struct ElfSectHeader32 *symtab = (struct ElfSectHeader32 *)(file + info->symtab_of);
	struct ElfSectHeader32 *strtab = (struct ElfSectHeader32 *)(file + info->strtab_of);

	int length = symtab->size / symtab->entsize;

	for (int i = 0; i < length; i++) {
		struct ElfSym32 *sym = (struct ElfSym32 *)(file + symtab->offset + (sizeof(struct ElfSym32) * i));
		if (sym->name != 0) {
			char *curr = (char *)file + strtab->offset + sym->name;
			if (sym->shndx != 0) {
				if (!strcmp(name, curr)) {
					struct ElfSectHeader32 *l = get_elf_head(file, sym->shndx);
					return l->offset + sym->value;
				}
			}
		}
	}

	return NULL;
}

uint32_t linker_exec(void *file, struct ElfFileInfo *info) {
	uintptr_t main = linker_get_symbol(file, info, "main");
	if (main == NULL) {
		printf("main() not found");
		return 1;
	}

	typedef int func(void);
	func* entry = (func*)(file + main);
	printf("Calling main...\n");
	return entry();
}
