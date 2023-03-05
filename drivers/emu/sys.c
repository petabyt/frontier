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

void fsyscall(int v) {
	((char *)SYS_CTL)[0] = v;
}

void msleep(int ms) {
	((int *)SYS_R0)[0] = ms;
	fsyscall(SYS_SLEEP);
}

void exit(int status) {
	fsyscall(SYS_EXIT);
}

void abort() {
	fsyscall(SYS_EXIT);
}

int _kill(int pid, int sig) {
	return -1;
}

void sys_dump() {
	fsyscall(SYS_BARF);
}

void sys_init_bmp() {
	fsyscall(SYS_SETUP_BMP);
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
}

void get_key_status() {
	
}

int _gettimeofday() {
	return 0;
}

int _getpid() {
	return -1;
}
