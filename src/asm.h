// Dangerous patching and crashing macros for hardcore debugging
#ifndef HIJACK_H
#define HIJACK_H

#include <stdint.h>

void asm_gen_branch(void *base, void *to);
void asm_gen_call(void *base, void *to);
void asm_gen_nop(void *buffer);
void asm_gen_nops(void *buffer, int n);

#endif
