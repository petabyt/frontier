#include <string.h>
#include <stdint.h>
#include <stdio.h>

#include <io.h>
#include <bmp.h>

int entry() {
	uart_dbg("Drawing...");
	bmp_clear(0x404a6e);
	bmp_fill_rect(10, 10, SCREEN_WIDTH - 20, 300, 0x1c202e);
	font_print_string(15, 15, "Hello from the custom emulator");
	font_print_string(15, 20+7+4, "Testing");
	bmp_apply();
	uart_dbg("Done...");
	msleep(1000);
	sys_exit();
	return 0;
}
