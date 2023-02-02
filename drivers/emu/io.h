#define UART_OUT_CHAR 0x40000001
#define SYS_CTL 0x40000000
#define SYS_SLEEP 0x40000004
#define SYS_KEY 0x4000000b
#define SYS_MOUSE_DOWN 0x4000000e
#define SYS_MOUSE_X 0x40000012
#define SYS_MOUSE_Y 0x40000016

void uart_char(char c);
void uart_str(char *string);
void uart_log(char *format, ...);

#define SCREEN_BUFFER 0x40000100

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

void sys_exit();
void sys_dump();
void msleep(int ms);
void sys_init_bmp();
// void sys_init_bmp()

int sys_check_mouse();
int sys_check_key(int key);

#define MEM_ALLOC_START 0x40000008
