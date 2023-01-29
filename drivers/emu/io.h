#define UART_OUT_CHAR 0x40000001
#define UART_SYS_CTL 0x40000000
#define UART_SYS_SLEEP 0x40000004

void uart_char(char c);
void uart_dbg(char *string);

#define SCREEN_BUFFER 0x40000100

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

void sys_exit();
void msleep(int ms);
