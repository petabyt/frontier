#include <sys/stat.h>
#include <unicorn/unicorn.h>
#include <fcntl.h>
#include <io.h>

#define E_STACK_SIZE 1000000

#define E_RAM (8 * 1024 * 1024)
#define E_ADDRESS 0x0

#define E_SCREEN_OF 0x100

uint32_t alloc_start = 0;

FILE *temp_fp = NULL;

struct Keys {
	int last_key;
	int mouse_x;
	int mouse_y;
	int mouse_down;
}keys;

struct IOLastRegs {
	uint32_t r0;
	uint32_t r1;
	uint32_t r2;
}io_regs;

#include "bmp.c"

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

	void *buf = malloc(E_RAM);
	uc_mem_read(uc, 0, buf, E_RAM);
	FILE *f = fopen("dump", "w");
	if (f == NULL) return;
	fwrite(buf, 1, E_RAM, f);
	fclose(f);
	free(buf);
}

static uint64_t mmio_reads(uc_engine *uc, uint64_t offset, unsigned size, void *user_data) {
	switch (offset) {
	case 8:
		return alloc_start;
	case 0xc:
		CNFGHandleInput();
		return keys.last_key;
	case 0x10:
		CNFGHandleInput();
		return keys.mouse_down;
	case 0x14:
		CNFGHandleInput();
		return keys.mouse_x;
	case 0x18:
		CNFGHandleInput();
		return keys.mouse_y;
	case 0x1c:
		return io_regs.r0;
	}

	return 0x0;
}

void mem_get_string(uc_engine *uc, int of, char *string, int max) {
	char buf[1];
	int i = 0;
	while (1) {
		uc_mem_read(uc, of, buf, 1);
		string[i] = buf[0];
		i++;
		if (buf[0] == 0) {
			return;
		}
		of++;
	}
}

void mmio_syscall(uc_engine *uc, int value) {
	if (value == SYS_EXIT) {
		uc_close(uc);
		exit(0);
	} else if (value == SYS_RENDER) {
		CNFGUpdateScreenWithBitmap(screen, SCREEN_WIDTH, SCREEN_HEIGHT);
		//CNFGSwapBuffers();
	} else if (value == SYS_BARF) {
		barf(uc);
	} else if (value == SYS_SETUP_BMP) {
		screen = malloc(SCREEN_WIDTH * SCREEN_HEIGHT * 4);
		CNFGSetup("Rigged Emulator", SCREEN_WIDTH, SCREEN_HEIGHT);
	} else if (value == SYS_SLEEP) {
		usleep(io_regs.r0 * 1000);
	} else if (value == SYS_FOPEN) {
		if (io_regs.r1 == 0x10000) {
			io_regs.r1 = 0x8000;
		}
	
		char filename[64];
		mem_get_string(uc, io_regs.r0, filename, sizeof(filename));
		printf("%s %lX\n", filename, io_regs.r1);
		io_regs.r0 = open(filename, io_regs.r1, io_regs.r2);
	} else if (value == SYS_FWRITE) {
		char *buffer = malloc(io_regs.r1);
		uc_mem_read(uc, io_regs.r1, buffer, io_regs.r2);
		io_regs.r0 = write(io_regs.r0, buffer, io_regs.r1);
	} else if (value == SYS_FILE_SIZE) {
		char path[64];
		mem_get_string(uc, io_regs.r0, path, sizeof(path));
		struct stat s;
		stat(path, &s);
		io_regs.r0 = s.st_size;
	} else if (value == SYS_FREAD) {
		char *buffer = malloc(io_regs.r2);
		ssize_t x = read(io_regs.r0, buffer, io_regs.r2);
		uc_mem_write(uc, io_regs.r1, buffer, io_regs.r2);
		free(buffer);
		io_regs.r0 = x;
	} else if (value == SYS_FCLOSE) {
		io_regs.r0 = close(io_regs.r0);
	}
}

static void mmio_writes(uc_engine *uc, uint64_t offset, unsigned size, uint64_t value, void *user_data) {
	if (offset >= E_SCREEN_OF && offset <= E_SCREEN_OF + (SCREEN_WIDTH * SCREEN_HEIGHT * 4)) {
		screen[(offset - 0x100) / 4] = (uint32_t)value;
		return;
	}

	switch (offset) {
	case 0:
		mmio_syscall(uc, value);
		break;
	case 1:
	case 2:
	case 3:
		putchar(value);
		break;
	case 4:
		io_regs.r0 = value;
		break;
	case 8:
		io_regs.r1 = value;
		break;
	case 12:
		io_regs.r2 = value;
		break;
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
	uc_mem_map(uc, 0, E_RAM, UC_PROT_ALL);

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
	int reg = length + E_STACK_SIZE;
	reg -= (reg % 0x8);
	uc_reg_write(uc, UC_ARM_REG_SP, &reg);

	// Align allocation start address
	alloc_start = reg + 0x100;
	alloc_start -= (reg % 0x8);

	// Set up IO regions
	uc_mmio_map(uc, 0x40000000, 0x10000000, mmio_reads, NULL,
		mmio_writes, NULL);

	err = uc_emu_start(uc, 0, E_RAM, 0, 0);
	if (err) {
		printf("Emulation failed: %u %s\n", err, uc_strerror(err));
		barf(uc);
	}
	
	uc_close(uc);

	return 0;
}
