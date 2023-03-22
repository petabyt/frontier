#ifndef F_H
#define F_H

// Frontier kernel functions

extern int sys_mem_in_use;

void sys_dump();
void sys_init_bmp();
void sys_init_mem();

void uart_char(char c);
void uart_str(char *string);
void uart_log(char *format, ...);

int sys_check_mouse();
int sys_check_key(int key);

#endif
