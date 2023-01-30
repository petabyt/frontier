#include <stdio.h>
#include <stdarg.h>
#include "io.h"

void sys_exit() {
	((char *)UART_SYS_CTL)[0] = 0;
}

void sys_dump() {
	((char *)UART_SYS_CTL)[0] = 2;
}

void sys_init_bmp() {
	((char *)UART_SYS_CTL)[0] = 3;
}

void msleep(int ms) {
	((int *)UART_SYS_SLEEP)[0] = ms;
}

void uart_char(char c) {
	((char *)UART_OUT_CHAR)[0] = c;
}

void uart_str(char *string) {
	while (*string != '\0') {
		uart_char(*string);
		string++;
	}
}

void uart_log(char *format, ...) {
	char buffer[128];
	va_list aptr;

	va_start(aptr, format);
	vsprintf(buffer, format, aptr);
	va_end(aptr);

	uart_str(buffer);
}
