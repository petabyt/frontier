#include <stdint.h>

#include <bmp.h>
#include <io.h>

/*

x, y == 0 is start when available
width, height == 0 is fill avilable space

x, y between 0 and 4 is start at quadrant

width, height between 0 and 4 is end at quadrant

draw selectors, labels, and buttons, after the last container

ui_draw_container(0, 0, 0, 100, color);
ui_draw_selector("Hello, World", color);
*/

struct UiDimensions {
	int x[10];
	int y[10];
	int i[10];
	int cur;
}ui;

int ui_reset() {
	memset(&ui, 0, sizeof(ui));
}

void ui_draw_container(int x, int y, int width, int height, int color) {
	if (x == 0) {
		x = ui.x[ui.cur];
	}

	if (y == 0) {
		y = ui.y[ui.cur];
	}

	if (width == 0) {
		width = SCREEN_WIDTH - ui.x[ui.cur];
	}

	if (height == 0) {
		height = SCREEN_HEIGHT - ui.y[ui.cur];
	}

	bmp_fill_rect(x, y, width, height, color);

	if (x + width == SCREEN_WIDTH) {
		x = 0;
		width = 0;
	}

	ui.x[ui.cur] = x + width;
	ui.y[ui.cur] = y + height;
	ui.i[ui.cur] = 0;
	ui.cur++;
	ui.x[ui.cur] = x;
	ui.y[ui.cur] = y;
	ui.i[ui.cur] = 0;
}

void ui_draw_text(char *text, int color) {
	if (ui.i[ui.cur] == 0) {
		ui.x[ui.cur] += 7;
		ui.y[ui.cur] += 7;
		ui.i[ui.cur] = 1;
	} else {
		ui.y[ui.cur] += 20;
	}

	font_print_string(ui.x[ui.cur], ui.y[ui.cur], text, color);
}

void ui_end_container() {
	ui.cur--;
}
