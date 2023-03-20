// Global symbol mananger, see fpack.c
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

extern uint32_t _symbol_table_start;

struct SymEntry {
	uintptr_t addr;
	uint16_t length;
	char string[];
};

int global_syms_length = 0;
struct GlobalSyms {
	uintptr_t table;
	int length;
}global_syms[10];

int table_size(int i) {
	void *tmp = (void *)(global_syms[i].table);

	int of = 0;
	while (1) {
		struct SymEntry *x = (struct SymEntry *)(tmp + of);
		if (x->addr == 0) return of;
		of += 8 + x->length;
	}
}

int sym_new(char *name, uint32_t value) {
	int length = table_size(global_syms_length - 1);

	// Allocate a new table if necessary
	int predicted_size = length - (int)strlen(name) + (int)sizeof(struct SymEntry);
	if (predicted_size >= length) {
		global_syms[global_syms_length].table = (uintptr_t)malloc(10000);
		global_syms[global_syms_length].length = 10000;
		memset((void *)global_syms[global_syms_length].table, 0, 10000);
		global_syms_length++;
	}

	length = table_size(global_syms_length - 1);
	struct SymEntry *x = (struct SymEntry *)(global_syms[global_syms_length - 1].table + length);
	x->addr = value;
	x->length = strlen(name);
	memcpy(x->string, name, x->length);

	return 0;
}

int sys_init_syms() {
	global_syms[0].table = ((uintptr_t)(&_symbol_table_start));
	global_syms[0].length = 0;
	global_syms_length = 1;
	return 0;
}

void *sym(const char *name) {
	int of = 0;
	while (1) {
		struct SymEntry *x = (struct SymEntry *)((uintptr_t)(&_symbol_table_start) + of);
		if (!strncmp(name, x->string, x->length)) {
			return (void *)x->addr;
		} else {
			of += 8 + x->length;
		}
	}

	return NULL;
}
