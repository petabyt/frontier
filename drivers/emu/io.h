#define UART_OUT_CHAR 0x40000001
#define UART_SYS_CTL 0x40000000
#define UART_SYS_SLEEP 0x40000004

void uart_char(char c);
void uart_str(char *string);
void uart_log(char *format, ...);

#define SCREEN_BUFFER 0x40000100

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

void sys_exit();
void sys_dump();
void msleep(int ms);
// void sys_init_bmp()

#define MEM_ALLOC_START 0x40000008
