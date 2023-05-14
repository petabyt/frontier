#include <bmp.h>

void fault_screen() {
	bmp_clear(0x33bb);

	font_print_string(10, 10, "A problem has been detected and FrontierOS has been", 0xffffff);
	font_print_string(10, 25, "shut down to prevent damage to your camera.", 0xffffff);

	char buffer[64];
	sprintf(buffer, "Testing 0x%X", 0xbeef);
	font_print_string(20, 50, buffer, 0xffffff);
}
