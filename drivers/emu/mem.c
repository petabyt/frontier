// Barely working 32 bit memory manager
#include <stdint.h>
#include <string.h>
#include "io.h"

void _sbrk() {}

#define MAX_ALLOCS 1024

struct Alloc {
	struct Bit {
		uintptr_t address;
		uint32_t size;
		unsigned char active;
	}bits[MAX_ALLOCS];
	uint32_t data_start;
};

struct Alloc *allocs = 0;

uintptr_t fixup_address(uintptr_t addr) {
	return addr + (0xf - (addr & 0xf) + 1);
}

void mem_init() {
	allocs = (struct Alloc *)(((uintptr_t *)MEM_ALLOC_START)[0]);
	memset(allocs, 0, sizeof(struct Alloc));

	// Allocate an initial stub
	allocs->bits[0].address = fixup_address((uintptr_t)(&allocs->data_start));
}

void *alloc(long unsigned int size) {
	size += 0xa;
	if (size == 0) {
		return 0;
	}

	uint32_t last_addr = 0;
	int last = 0;
	for (int i = 0; i < MAX_ALLOCS; i++) {
		if (allocs->bits[i].address + allocs->bits[i].size > last_addr) {
			last = i;
			last_addr = allocs->bits[i].address + allocs->bits[i].size;
		}

		if (allocs->bits[i].active) continue;

		if (allocs->bits[i].size >= size) {
			allocs->bits[i].size = size;
			allocs->bits[i].active = 1;
			return (void *)((allocs->bits[i].address));
		}
	}

	allocs->bits[last].address = fixup_address(last_addr);
	allocs->bits[last].active = 1;
	allocs->bits[last].size = size;
	return ((void *)(allocs->bits[last].address));
}

void *malloc(long unsigned int size) {
	void *x = alloc(size);
	return x;
}

void free(void *ptr) {
	for (int i = 0; i < MAX_ALLOCS; i++) {
		if ((uintptr_t)ptr == allocs->bits[i].address) {
			allocs->bits[i].active = 0;
			return;
		}
	}	
}

void *calloc(long unsigned int n, long unsigned int size) {
	void *x = malloc(n * size);
	memset(x, 0, n * size);
	return x;
}

void *realloc(void *buf, long unsigned int n) {
	if (buf == NULL) {
		return malloc(n);
	} else {
		free(buf);
		void *new = malloc(n);
		memcpy(new, buf, n);
		return new;
	}
}
