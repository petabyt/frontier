#include <stdint.h>
#include <bmp.h>

#include "font.h"

void bmp_fill_rect(int x1, int y1, int x2, int y2, uint32_t rgb) {
	x2 += x1;
	y2 += y1;
	for (int x = x1; x < x2; x++) {
		for (int y = y1; y < y2; y++) {
			bmp_pixel(x, y, rgb);
		}
	}
}

int font_print_char(int x, int y, char c, int color) {
	// Loop to "null terminator character"
	int match = 0;
	for (int l = 0; font[l].letter != 0; l++) {
		if (font[l].letter == c) {
			match = l;
			break;
		}
	}

	if (c == 'p' || c == 'g' || c == 'j') {
		y += 1;
	}

	// Loop through 7 high 5 wide monochrome font
	int maxLength = 0;
	for (int py = 0; py < 7; py++) {
		for (int px = 0; px < 5; px++) {
			if (font[match].code[py][px] == '#') {
				//bmp_fill_rect((x + px + 1) * 2, (y + py) * 2, 1, 2, 0x999999);
				bmp_fill_rect((x + px) * 2, (y + py) * 2, 2, 2, color);

				// Dynamix width character spacing
				if (px > maxLength) {
					maxLength = px;
				}
			}
		}
	}

	return maxLength;
}

int font_print_string(int x, int y, char *string, int color) {
	int cx = x;
	int cy = y;

	for (int c = 0; string[c] != '\0'; c++) {
		int length;
		if (string[c] == ' ') {
			length = 5;
		} else {
			length = font_print_char(cx, cy, string[c], color) + 3;
		}

		cx += length;
	}

	return cy;
}
