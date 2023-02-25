const char *app_name = "myappname";

char *foobar = "hleloworld";

void print(char *);

extern int fat;

int main() {
	fat = 1;
	print(foobar);
}
