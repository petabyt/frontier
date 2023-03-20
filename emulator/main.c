#include <unicorn/unicorn.h>

#include <pack.h>
#include <emulator.h>

int help() {
	printf(
		"Frontier build system utility\n"
		"-i\tSpecify input file\n"
		"-o\tSpecify output file\n"
		"-s\tAppend symbols from single relocatable file\n"
		"-a\tPrepend file data for app\n"
		"-e\tRun input file in graphical emulator\n"
	);

	return 0;
}

int main(int argc, char *argv[]) {
	char *input = "os.elf";
	char *output = "os.bin";
	for (int i = 0; i < argc; i++) {
		if (argv[i][0] != '-') continue;
		switch (argv[i][1]) {
		case 'i':
			input = argv[i + 1];
			break;
		case 'o':
			output = argv[i + 1];
			break;
		case 's':
			// Append single row of symbols from file
			return add_syms(input, output);
		case 'a': {
			char *content = json_fread(argv[i + 1]);
			struct AppMetaData amd;
			gen_app_meta(&amd, content);
		} break;
		case 'e':
			return emulator(input);
		case 'h':
			return help();
		}
	}

	puts("No valid args");
	return 0;
}
