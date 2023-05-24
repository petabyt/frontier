#include <stdint.h>

void generate_branch(void *base, void *to, void *buffer) {
	((uint32_t*)buffer)[0] = ((to - base - 8) >> 2) & 0x00ffffff;
	((uint8_t*)buffer)[3] = 0xea;
}

void generate_call(void *base, void *to, void *buffer) {
	generate_branch(base, to, buffer);
	((uint8_t*)buffer)[3] = 0xeb;
}

void asm_gen_nop(void *buffer) {
	((uint32_t*)buffer)[0] = 0xe320f000; // NOP
}

void asm_gen_nops(void *buffer, int n) {
	for (int i = 0; i < n; i++) {
		asm_gen_nop(buffer + (n * 4));
	}
}	
