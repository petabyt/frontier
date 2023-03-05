// Frontier ELF Linker

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <elf.h>
#include <linker.h>

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

// char *elf_get_of_string(void *file, struct ElfFileInfo *info, uint32_t of) {
	// struct ElfSectHeader32 *s = (struct ElfSectHeader32 *)(file + info->strtab_of);
	// return (char *)file + s->offset + of;
// }

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
		printf("    Offset: %lX\n", relocs[i].offset);
		printf("    Info: %ld\n", type);

		char *name = (char *)(file + str->offset + syms[index].name);

		uint32_t *target_loc = (void *)(file + target->offset + relocs[i].offset);
		if (type == R_ARM_CALL) {
			if (syms[index].value == 0 && syms[index].shndx == 0) {
				printf("    Generating call for %s\n", name);
				void *call = fdlsym(NULL, name);
				if (call == NULL) {
					printf("NULL %s\n", name);
					return 1;
				}
				asm_gen_call(target_loc, call);
			} else {
				asm_gen_call(target_loc, file + syms[index].value + get_elf_head(file, syms[index].shndx)->offset);
			}
		} else if (type == R_ARM_ABS32) {
			target_loc[0] = file + target_loc[0] + get_elf_head(file, syms[index].shndx)->offset;
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
		return LINK_BAD_FORMAT;
	}
	
	// Read program table header entries
	for (int i = 0; i < h->phnum; i++) {
		struct ElfProgHeader32 *p = (struct ElfProgHeader32 *)
			(file + h->phoff + (i * h->phentsize));
	}

	struct ElfSectHeader32 *names = (struct ElfSectHeader32 *)
				(file + h->shoff + (h->shstrndx * h->shentsize));
	printf("Names offset %lX\n", names->offset);

	for (int i = 0; i < h->shnum; i++) {
		struct ElfSectHeader32 *s = (struct ElfSectHeader32 *)
			(file + h->shoff + (i * h->shentsize));

		char *sect_name = (char *)file + names->offset + s->name;

		printf("Table: %s\n", sect_name);
		printf("    Table offset: %lX\n", s->offset);
		if (s->link != 0) {
			struct ElfSectHeader32 *s2 = (struct ElfSectHeader32 *)
				(file + h->shoff + (s->link * h->shentsize));
			printf("    Links to %s\n", (char *)file + names->offset + s2->name);
		}

		if (s->info != 0) {
			struct ElfSectHeader32 *s2 = (struct ElfSectHeader32 *)
				(file + h->shoff + (s->info * h->shentsize));
			printf("    Info to %s\n", (char *)file + names->offset + s2->name);
		}

		if (s->type == SHT_PROGBITS) {
			info->max_exec_size += s->size;
		} else if (s->type == SHT_REL) {
			if (linker_relocate(file, info, s)) {
				return 1;
			}
		}

		uint32_t of = h->shoff + (i * h->shentsize);
	}

	return 0;
}

uint32_t linker_exec(void *file, struct ElfFileInfo *info) {
	struct ElfHeader32 *h = (struct ElfHeader32 *)file;
	typedef void func(void);
	func* entry = (func*)(file + h->entry);
	printf("Calling main...\n");
	entry();
}
