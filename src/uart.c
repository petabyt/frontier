#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <sys.h>
#include <js.h>

void uart_welcome() {
	printf("FrontierOS 32 bit ARM\n");
	printf("Written by Daniel C - https://danielc.dev/\n");
	//cpu_info();
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

			puts(buffer);
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
