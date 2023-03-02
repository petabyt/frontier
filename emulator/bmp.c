#define CNFG_IMPLEMENTATION
#include "rawdraw_sf.h"

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

uint32_t *screen;

void HandleKey(int keycode, int bDown) {
	if (bDown == 1) {
		keys.last_key = keycode;
	} else {
		keys.last_key = 0;
	}
}

void HandleButton( int x, int y, int button, int bDown ) {
	keys.mouse_down = bDown;
}

void HandleMotion( int x, int y, int mask ) {
	keys.mouse_x = x / 2;
	keys.mouse_y = y / 2;
}

void HandleDestroy() {
	puts("Destroy");
}
