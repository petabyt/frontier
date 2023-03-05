// Minimal backend code for GCC malloc
#include <stdint.h>
#include <string.h>

#include "io.h"

void mem_init() {}

void *heap_end = 0;
void *_sbrk(int incr) {
	if (heap_end == 0) {
		heap_end = (void *)(((uintptr_t *)MEM_ALLOC_START)[0]);
	}

	void *prev = heap_end;

	heap_end += incr;
	return prev;
}
