#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include <string.h>

#include <buttons.h>

#include "sys.h"
#include "io.h"

int sys_mem_in_use = 0;

void sys_key_status() {}

int sys_check_key(int key) {
	if (key == SYS_BUTTON_QUIT) {
		key = 27;
	} else if (key == SYS_BUTTON_DOWN) {
		key = 40;
	} else if (key == SYS_BUTTON_UP) {
		key = 38;
	} else if (key == SYS_BUTTON_LEFT) {
		key = 37;
	} else if (key == SYS_BUTTON_RIGHT) {
		key = 39;
	}

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

void exit(int status) {
	((char *)SYS_CTL)[0] = SYS_EXIT;
	status = status;
}

void abort() {
	syscall(SYS_EXIT);
}

int _kill(int pid, int sig) {
	return -1;
}

void sys_dump() {
	syscall(SYS_BARF);
}

void uart_char(char c) {
	((char *)UART_OUT_CHAR)[0] = c;
}

void uart_str(const char *string) {
	while (*string != '\0') {
		uart_char(*string);
		string++;
	}
}

int printf(const char *format, ...) {
	char buffer[128];
	va_list aptr;

	va_start(aptr, format);
	vsprintf(buffer, format, aptr);
	va_end(aptr);

	uart_str(buffer);

	return 0;
}

int puts(const char *x) {
	uart_str(x);
	uart_char('\n');
	return 0;
}

int _gettimeofday() {
	return 0;
}

int _getpid() {
	return -1;
}
