#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <sys.h>
#include <js.h>
#include <sym.h>

#define REGBITS(i, a, b) (i >> b) & (((a - b + 1) * 32) - 1)

uint32_t asm_get_cpuinfo();

void cpu_info() {
	uint32_t i = asm_get_cpuinfo();
	printf("Implementor: %c\n", (char)REGBITS(i, 31, 24));
	//printf("Bit 23: %lX\n", (i >> 23) & 1);
	printf("Arch: %lX\n", REGBITS(i, 19, 16));
	printf("Part number: %lX\n", REGBITS(i, 15, 4));
	printf("Revision: %lX\n", REGBITS(i, 3, 0));
}

void uart_welcome() {
	printf("FrontierOS 32 bit ARM\n");
	printf("Written by Daniel C - https://danielc.dev/\n");
	cpu_info();
}

int prompt_line() {
	char buffer[64];
	int curr = 0;
	while (1) {
		char c = getchar();
		if (c == (char)255) {
			exit(0);
		}

		buffer[curr] = c;

		if (c == '\n') {
			buffer[curr] = '\0';

			puts(js_run(buffer));
			return 0;
		}

		curr++;
	}
}

void uart_prompt() {
	while (1) {
		uart_str("$ ");
		prompt_line();
	}
}
