#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include <string.h>
#include "io.h"

int sys_check_key(int key) {
	return ((char *)SYS_KEY)[0] == key;
}

int sys_check_mouse() { return ((char *)SYS_MOUSE_DOWN)[0]; }
int sys_mouse_x() { return ((int *)SYS_MOUSE_X)[0]; }
int sys_mouse_y() { return ((int *)SYS_MOUSE_Y)[0]; }

void syscall(int v) {
	((char *)SYS_CTL)[0] = v;
}

void msleep(int ms) {
	((int *)SYS_R0)[0] = ms;
	syscall(SYS_SLEEP);
}

void sys_exit() {
	syscall(SYS_EXIT);
}

void sys_dump() {
	syscall(SYS_BARF);
}

void sys_init_bmp() {
	syscall(SYS_SETUP_BMP);
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

void get_key_status() {
	
}

struct SymEntry {
	uintptr_t addr;
	short length;
	char string[];
};

extern uint32_t _symbol_table_start;
void *mdlsym(void *handle, const char *name) {
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
