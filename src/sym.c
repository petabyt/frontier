// Global runtime symbol mananger
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <sys.h>

extern uint32_t _symbol_table_start;

#pragma pack(push, 1)

struct TblEntry {
	uint32_t addr;
	uint16_t length;
	uint8_t type;
	uint8_t etc;
	char string[];
};

#define DEFAULT_TABLE_SIZE 10000

// A global array of symbol tables - new tables are added
// and allocated on the fly
int global_syms_length = 0;
struct GlobalSyms {
	uintptr_t table;
	int length;
}global_syms[10];

#pragma pack(pop)

int table_size(int i) {
	void *tmp = (void *)(global_syms[i].table);

	int of = 0;
	while (1) {
		struct TblEntry *x = (struct TblEntry *)(tmp + of);
		if (x->addr == 0) return of;
		of += 8 + x->length;
	}
}

int sym_new(char *name, uint32_t value) {
	int length = table_size(global_syms_length - 1);

	// Allocate a new table if necessary
	int predicted_size = length - (int)strlen(name) + (int)sizeof(struct TblEntry);
	if (predicted_size >= length) {
		global_syms[global_syms_length].table = (uintptr_t)malloc(DEFAULT_TABLE_SIZE);
		global_syms[global_syms_length].length = DEFAULT_TABLE_SIZE;
		memset((void *)global_syms[global_syms_length].table, 0, DEFAULT_TABLE_SIZE);
		global_syms_length++;
	}

	length = table_size(global_syms_length - 1);
	struct TblEntry *x = (struct TblEntry *)(global_syms[global_syms_length - 1].table + length);
	x->addr = value;
	x->length = strlen(name);
	memcpy(x->string, name, x->length);

	return 0;
}

int sys_init_syms() {
	global_syms[0].table = ((uintptr_t)(&_symbol_table_start));
	global_syms[0].length = 0;
	global_syms_length = 1;

	sys_debug("Initialized internal symbols\n");
	sys_debug("Table length: %d\n", table_size(0));
	
	return 0;
}

void *sym(const char *name) {
	for (int i = 0; i < global_syms_length; i++) {
		int of = 0;
		uintptr_t table = global_syms[i].table;
		if (global_syms[i].length == 0) {
			// Read until a blank entry
			while (1) {
				struct TblEntry *x = (struct TblEntry *)(table + of);
				if (x->length == 0) return NULL;
				if (!strncmp(name, x->string, x->length)) {
					return (void *)((uintptr_t)x->addr);
				} else {
					of += 8 + x->length;
				}
			}
		} else {
			for (int i = 0; i < global_syms[i].length; i++) {
				struct TblEntry *x = (struct TblEntry *)(table + of);
				if (x->length == 0) return NULL;
				if (!strncmp(name, x->string, x->length)) {
					return (void *)((uintptr_t)x->addr);
				} else {
					of += 8 + x->length;
				}
			}			
		}
	}

	return NULL;
}
