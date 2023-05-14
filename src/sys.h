#ifndef F_H
#define F_H
#include <stdint.h>

// Kernel API

extern int sys_mem_in_use;
extern int sys_mem_available;

void sys_dump();
void sys_init_bmp();
void sys_init_mem();

void uart_char(char c);
void uart_str(char *string);
void uart_log(char *format, ...);

#define SYS_BUTTON_NONE 0
#define SYS_BUTTON_QUIT 1
#define SYS_BUTTON_UP 2
#define SYS_BUTTON_DOWN 3
#define SYS_BUTTON_LEFT 4
#define SYS_BUTTON_RIGHT 5
#define SYS_BUTTON_OK 6

int sys_check_mouse();
int sys_check_key(int key);

int sys_segment(void *start, uint32_t length);

int sys_map_mem(void *start, uint32_t length, int type);

#endif
