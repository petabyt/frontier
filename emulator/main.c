#include <unicorn/unicorn.h>

//Make it so we don't need to include any other C files in our build.
#define CNFG_IMPLEMENTATION
#include "rawdraw_sf.h"

void HandleKey( int keycode, int bDown ) { }
void HandleButton( int x, int y, int button, int bDown ) { }
void HandleMotion( int x, int y, int mask ) { }
void HandleDestroy() { }

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

#define SCREEN_OF 0x100

uint32_t *screen;

static uint64_t mmio_reads(uc_engine *uc, uint64_t offset, unsigned size, void *user_data) {
	return 0x0;
}

static void mmio_writes(uc_engine *uc, uint64_t offset, unsigned size, uint64_t value, void *user_data) {
	if (offset >= SCREEN_OF && offset <= SCREEN_OF + (SCREEN_WIDTH * SCREEN_HEIGHT * 4)) {
		screen[(offset - 0x100) / 4] = (uint32_t)value;
		return;
	}

	switch (offset) {
	case 0:
		if (value == 0) {
			uc_close(uc);
			exit(0);
		} else if (value == 1) {
			CNFGBlitImage(screen, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
			CNFGSwapBuffers();
		}
	case 1:
		printf("%c", (char)value);
		return;
	case 4:
		usleep(value * 1000);
		return;
	}
}

#define RAM (2 * 1024 * 1024)
#define ADDRESS 0x0

int main(int argc, char *argv[]) {
	char *filename = "os.bin";
	if (argc > 1) {
		filename = argv[1];
	}
	
	uc_engine *uc;
	uc_err err;

	err = uc_open(UC_ARCH_ARM, UC_MODE_ARM, &uc);
	if (err != UC_ERR_OK) {
		printf("Failed\n");
		return 1;
	}

	// Map 2MB memory
	uc_mem_map(uc, 0, RAM, UC_PROT_ALL);

	FILE *f = fopen(filename, "r");
	if (f == NULL) {
		printf("File not found\n");
		return 1;
	}

	fseek(f, 0, SEEK_END);
	int length = ftell(f);
	fseek(f, 0, SEEK_SET);
	char *buffer = malloc(length);
	fread(buffer, length, 1, f);
	fclose(f);

	uc_mem_write(uc, 0, buffer, length);	
	free(buffer);

	// 100k of stack (grows backwards)
	int reg = length + 100000;
	uc_reg_write(uc, UC_ARM_REG_SP, &reg);

	// Set up IO regions
	uc_mmio_map(uc, 0x40000000, 0x10000000, mmio_reads, NULL,
		mmio_writes, NULL);

	// Setup up the screen
	screen = malloc(SCREEN_WIDTH * SCREEN_HEIGHT * 4);
	CNFGSetup("Rigged Emulator", SCREEN_WIDTH, SCREEN_HEIGHT);

	uc_emu_start(uc, 0, RAM, 0, 0);
	
	uc_reg_read(uc, UC_ARM_REG_R0, &reg);
	printf("r0: 0x%X\n", reg);

	uc_close(uc);

	return 0;
}
