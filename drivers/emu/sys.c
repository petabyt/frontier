#include "io.h"

void sys_exit() {
	((char *)UART_SYS_CTL)[0] = 0;
}

void msleep(int ms) {
	((int *)UART_SYS_SLEEP)[0] = ms;
}

void uart_char(char c) {
	((char *)UART_OUT_CHAR)[0] = c;
}

void uart_dbg(char *string) {
	uart_char('>');
	uart_char(' ');
	while (*string != '\0') {
		uart_char(*string);
		string++;
	}
	uart_char('\n');
}
