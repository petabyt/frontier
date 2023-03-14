#include <stdint.h>

void asm_gen_branch(void *base, void *to) {
	((uint32_t*)base)[0] = ((to - base - 8) >> 2) & 0x00ffffff;
	((uint8_t*)base)[3] = 0xea;
}

void asm_gen_call(void *base, void *to) {
	asm_gen_branch(base, to);
	((uint8_t*)base)[3] = 0xeb;
}

void asm_gen_nop(void *buffer) {
	((uint32_t*)buffer)[0] = 0xe320f000; // NOP
}

void asm_gen_nops(void *buffer, int n) {
	for (int i = 0; i < n; i++) {
		asm_gen_nop(buffer + (n * 4));
	}
}	
