#include <unicorn/unicorn.h>

static uint64_t mmio_reads(uc_engine *uc, uint64_t offset, unsigned size, void *user_data) {
	return 0x0;
}

static void mmio_writes(uc_engine *uc, uint64_t offset, unsigned size, uint64_t value, void *user_data) {
	if (offset == 1) {
		printf("%c", (char)value);
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
	uc_mem_map(uc, 0, 2 * 1024 * 1024, UC_PROT_ALL);

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

	int reg = length + 64;
	uc_reg_write(uc, UC_ARM_REG_SP, &reg);

	reg = length;
	uc_reg_write(uc, UC_ARM_REG_LR, &reg);

	uc_mmio_map(uc, 0x40000000, 0x1000, mmio_reads, NULL,
		mmio_writes, NULL);

	uc_emu_start(uc, 0, length, 0, 0);
	
	uc_reg_read(uc, UC_ARM_REG_R0, &reg);
	printf("r0: 0x%X\n", reg);

	uc_close(uc);

	return 0;
}
