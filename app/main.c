#include <stdio.h>

int calculate(int, int);

const char *foo = "Hello, WOrld";

extern char *strn;

int main() {
	printf("Hello, ELF World\n");
	printf("Test: %d\n", calculate(1, 1));
}

int calc() {
	return 1231;
}
