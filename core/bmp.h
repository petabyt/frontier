#ifndef BMP_H
#define BMP_H

void bmp_clear(uint32_t rgb);
void bmp_apply();
void bmp_test();
void bmp_fill_rect(int x1, int y1, int x2, int y2, uint32_t rgb);
void bmp_pixel(int x, int y, uint32_t rgb);

int font_print_char(int x, int y, char c, int color);
int font_print_string(int x, int y, char *string, int color);

#endif
