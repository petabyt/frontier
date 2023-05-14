#ifndef UI_H
#define UI_H

typedef int ui_renderer();
int ui_frame(ui_renderer *);

int ui_reset();
void ui_container(int x, int y, int width, int height, int color);
int ui_button(char *text);
void ui_text(char *text, int color);
void ui_end_container();

#endif
