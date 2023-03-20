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
	} else if (key == SYS_BUTTON_RIGHT) {
		key = 39;
	} else if (key == SYS_BUTTON_OK) {
		key = 13;
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
	((int *)SYS_R0)[0] = status;
	syscall(SYS_EXIT);
}

void abort() {
	syscall(SYS_EXIT);
}

void sys_dump() {
	syscall(SYS_BARF);
}

int sys_segment(uintptr_t start, uint32_t length) {
	((uintptr_t *)SYS_R0)[0] = start;
	((uint32_t *)SYS_R0)[1] = start;
	syscall(SYS_SEGMENT);
}

int sys_get_ticks() {
	return ((uint32_t *)SYS_TICKS_MS)[0];
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
	char buffer[1024];
	va_list aptr;

	va_start(aptr, format);
	vsnprintf(buffer, sizeof(buffer), format, aptr);
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

int _kill(int pid, int sig) {
	return -1;
}
