#include <stdint.h>
#include <string.h>

struct SymEntry {
	uintptr_t addr;
	short length;
	char string[];
};

extern uint32_t _symbol_table_start;
void *fdlsym(void *handle, const char *name) {
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
