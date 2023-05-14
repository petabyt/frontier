#include <stdint.h>
#include <string.h>

#include <bmp.h>
#include <sys.h>
#include <ui.h>

/*

x, y == 0 is start when available
width, height == 0 is fill avilable space

x, y between 0 and 4 is start at quadrant

width, height between 0 and 4 is end at quadrant

draw selectors, labels, and buttons, after the last container

ui_draw_container(0, 0, 0, 100, color);
ui_draw_selector("Hello, World", color);
*/

uint8_t select_x = 0;
uint8_t select_y = 0;
uint8_t done = 0;

static struct UiDimensions {
	int x[10];
	int y[10];
	int i[10];
	int cur;

	uint8_t index_x;
	uint8_t index_y;
	uint8_t interacted;
}ui;

int ui_reset() {
	memset(&ui, 0, sizeof(ui));
	return 0;
}

int ui_frame(ui_renderer *renderer) {
	ui_reset();
	bmp_clear(0);

	int key = 0;
	if (sys_check_key(SYS_BUTTON_QUIT)) {
		return 1;
	} else if (sys_check_key(SYS_BUTTON_DOWN)) {
		select_y++;
		key = SYS_BUTTON_DOWN;
		done = 0;
	} else if (sys_check_key(SYS_BUTTON_OK)) {
		key = SYS_BUTTON_OK;
		ui.interacted = 1;
		done = 0;
	}

	if (done == 0) {
		renderer();
		bmp_apply();
		done = 1;
		if (select_y >= ui.index_y) {
			select_y = 0;

			ui_reset();
			renderer();
			bmp_apply();
		}
	}

	if (key) {
		while (sys_check_key(key)) {
			msleep(5);
		}
	}

	return 0;
}

void ui_container(int x, int y, int width, int height, int color) {
	if (x == 0) {
		x = ui.x[ui.cur];
	}

	if (y == 0) {
		y = ui.y[ui.cur];
	}

	if (width == 0) {
		width = bmp_screen_width() - ui.x[ui.cur];
	}

	if (height == 0) {
		height = bmp_screen_height() - ui.y[ui.cur];
	}

	bmp_fill_rect(x, y, width, height, color);

	if (x + width == bmp_screen_width()) {
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
	ui.index_x++;
}

int ui_button(char *text) {
	int bg = 0x222222;
	if (ui.index_y == select_y) {
		bg = 0x666666;
	}

	bmp_fill_rect(ui.x[ui.cur], ui.y[ui.cur], bmp_screen_width(), 48, bg);
	bmp_string(ui.x[ui.cur] + 14, ui.y[ui.cur] + 14, text, 0xffffff);

	ui.x[ui.cur] = 0;
	ui.y[ui.cur] += 48;

	ui.index_y++;

	if (ui.index_y - 1 == select_y && ui.interacted) {
		return 1;
	}

	return 0;
}

void ui_text(char *text, int color) {
	if (ui.i[ui.cur] == 0) {
		// 7px padding?
		ui.x[ui.cur] += 7;
		ui.y[ui.cur] += 7;
		ui.i[ui.cur] = 1;
	} else {
		ui.y[ui.cur] += 20;
	}

	bmp_string(ui.x[ui.cur], ui.y[ui.cur], text, color);

	ui.y[ui.cur] += 20;
	//ui.index_y++;
}

void ui_end_container() {
	ui.cur--;
}
