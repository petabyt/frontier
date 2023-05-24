// perror equivelants
#include <stdio.h>
#include <stdarg.h>

static char error_buffer[128];

void sys_report_err(const char* format, ...) {
	va_list args;
	va_start(args, format);
	vsnprintf(error_buffer, sizeof(error_buffer), format, args);
	va_end(args);
}

char *sys_get_error() {
	return error_buffer;
}
