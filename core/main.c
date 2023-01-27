#include <string.h>
#include <stdint.h>

#define UART_OUT_CHAR 0x40000001

void uart_char(char c) {
	((char *)UART_OUT_CHAR)[0] = c;
}

void uart_dbg(char *string) {
	uart_char('>');
	uart_char(' ');
	while (*string != '\0') {
		uart_char(*string);
		string++;
	}
	uart_char('\n');
}

int entry() {
	char foo[123];
	strcpy(foo, "Hello ");
	strcat(foo, "World");	
	uart_dbg(foo);

	return 0;
}
