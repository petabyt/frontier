#include <stdint.h>
#include <bmp.h>

#include <font.h>

struct FontConfig {
	int size;
}font_conf = {
	2,
};

void bmp_fill_rect(int x1, int y1, int x2, int y2, uint32_t rgb) {
	x2 += x1;
	y2 += y1;
	for (int x = x1; x < x2; x++) {
		for (int y = y1; y < y2; y++) {
			bmp_pixel(x, y, rgb);
		}
	}
}

int bmp_char(int x, int y, char c, int color) {
	// Loop to "null terminator character"
	int match = 0;
	for (int l = 0; font[l].letter != 0; l++) {
		if (font[l].letter == c) {
			match = l;
			break;
		}
	}

	if (c == 'p' || c == 'g' || c == 'j') {
		y += font_conf.size;
	}

	// Loop through 7 high 5 wide monochrome font
	int maxLength = 0;
	for (int py = 0; py < 7; py++) {
		for (int px = 0; px < 5; px++) {
			if (font[match].code[py][px] == '#') {
				if (font_conf.size == 1) {
					bmp_pixel(x + px, y + py, color);
				} else {
					bmp_fill_rect((x + px) * font_conf.size, (y + py) * font_conf.size,
						font_conf.size, font_conf.size, color);
				}

				// Dynamix width character spacing
				if (px > maxLength) {
					maxLength = px;
				}
			}
		}
	}

	return maxLength;
}

int bmp_string(int x, int y, char *string, int color) {
	int cx = x;
	int cy = y;

	// for x2 size
	cy /= font_conf.size;
	cx /= font_conf.size;

	for (int c = 0; string[c] != '\0'; c++) {
		if (string[c] == '\n') {
			cx = x / font_conf.size;
			cy += 7 + font_conf.size;
			continue;
		}

		if (string[c] == ' ') {
			cx += 5;
		} else {
			cx += bmp_char(cx, cy, string[c], color) + 3;
		}
	}

	return cy;
}
