#include <stdint.h>
#include "io.h"

void bmp_apply() {
	((char *)UART_SYS_CTL)[0] = 1;
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
