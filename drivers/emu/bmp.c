#include <stdlib.h>
#include <stdint.h>

#include "emu.h"

void syscall(int v);

void sys_init_bmp() {
	syscall(SYS_SETUP_BMP);
}

int bmp_screen_width() {
	return SCREEN_WIDTH;
}

int bmp_screen_height() {
	return SCREEN_HEIGHT;
}

void bmp_apply() {
	((char *)SYS_CTL)[0] = 1;
}

void bmp_clear(uint32_t rgb) {
	for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++) {
		((uint32_t *)SCREEN_BUFFER)[i] = rgb;
	}
}

void bmp_pixel(int x, int y, uint32_t rgb) {
	int i = ((y * SCREEN_WIDTH) + x);
	((uint32_t *)SCREEN_BUFFER)[i] = rgb;
}

void bmp_test() {
	for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++) {
		((uint32_t *)SCREEN_BUFFER)[i] = i;
	}

	bmp_apply();
}
