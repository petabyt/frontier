#include <unicorn/unicorn.h>

//Make it so we don't need to include any other C files in our build.
#define CNFG_IMPLEMENTATION
#include "rawdraw_sf.h"

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

#define STACK_SIZE 1000000

#define RAM (8 * 1024 * 1024)
#define ADDRESS 0x0

uint32_t *screen;
uint32_t alloc_start = 0;

struct Keys {
	int last_key;
	int mouse_x;
	int mouse_y;
	int mouse_down;
}keys;

void HandleKey(int keycode, int bDown) {
	if (bDown == 1) {
		keys.last_key = keycode;
	} else {
		keys.last_key = 0;
	}
}

void HandleButton( int x, int y, int button, int bDown ) {
	keys.mouse_down = bDown;
}

void HandleMotion( int x, int y, int mask ) {
	keys.mouse_x = x/2;
	keys.mouse_y = y/2;
}

void HandleDestroy() {
	puts("Destroy");
}

void barf(uc_engine *uc) {
	int reg;

	uc_reg_read(uc, UC_ARM_REG_PC, &reg);
	printf("PC: %08X\n", reg);
	char buffer[128];
	sprintf(buffer, "arm-none-eabi-addr2line -e %s %X", "os.elf", (uint32_t)reg);
	system(buffer);

	puts("from");

	uc_reg_read(uc, UC_ARM_REG_LR, &reg);
	sprintf(buffer, "arm-none-eabi-addr2line -e %s %X", "os.elf", (uint32_t)reg);
	system(buffer);

	for (int i = 0; i < 10; i++) {
		uc_reg_read(uc, UC_ARM_REG_R0 + i, &reg);
		printf("r%d: 0x%X\n", i, reg);
	}

	void *buf = malloc(RAM);
	uc_mem_read(uc, 0, buf, RAM);
	FILE *f = fopen("dump", "w");
	fwrite(buf, 1, RAM, f);
	fclose(f);
}

static uint64_t mmio_reads(uc_engine *uc, uint64_t offset, unsigned size, void *user_data) {
	switch (offset) {
	case 8:
		return alloc_start;
	case 0xb:
		CNFGHandleInput();
		return keys.last_key;
	case 0xe:
		CNFGHandleInput();
		return keys.mouse_down;
	case 0x12:
		CNFGHandleInput();
		return keys.mouse_x;
	case 0x16:
		CNFGHandleInput();
		return keys.mouse_y;
	}

	//if (offset)

	return 0x0;
}

#define SCREEN_OF 0x100

static void mmio_writes(uc_engine *uc, uint64_t offset, unsigned size, uint64_t value, void *user_data) {
	if (offset >= SCREEN_OF && offset <= SCREEN_OF + (SCREEN_WIDTH * SCREEN_HEIGHT * 4)) {
		screen[(offset - 0x100) / 4] = (uint32_t)value;
		return;
	}

	switch (offset) {
	// General purpose sys call
	case 0:
		if (value == 0) {
			uc_close(uc);
			exit(0);
		} else if (value == 1) {
			CNFGBlitImage(screen, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
			CNFGSwapBuffers();
		} else if (value == 2) {
			barf(uc);
		} else if (value == 3) {
			// Setup up the screen
			screen = malloc(SCREEN_WIDTH * SCREEN_HEIGHT * 4);
			CNFGSetup("Rigged Emulator", SCREEN_WIDTH, SCREEN_HEIGHT);
		}
	case 1:
		printf("%c", (char)value);
		return;
	case 4:
		usleep(value * 1000);
		return;
	}
}

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
	int reg = length + STACK_SIZE;
	reg -= (reg % 0x8);
	uc_reg_write(uc, UC_ARM_REG_SP, &reg);

	alloc_start = reg + 0x100;
	alloc_start -= (reg % 0x8);

	// Set up IO regions
	uc_mmio_map(uc, 0x40000000, 0x10000000, mmio_reads, NULL,
		mmio_writes, NULL);

	err = uc_emu_start(uc, 0, RAM, 0, 0);
	if (err) {
		printf("Emulation failed: %u %s\n", err, uc_strerror(err));
		barf(uc);
	}
	
	uc_close(uc);

	return 0;
}
