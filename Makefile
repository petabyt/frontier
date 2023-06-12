all: os.bin build-emu
	emulator/emu.o -i os.bin -e

-include $(TOPL)/config.mak
ARCH?=emu
include arch.mak

TOPL=.
include common.mk

ARMLDFLAGS=-T Linker.ld

# Add multiple locations to find GCC and libc libs
ARMCCLOC?=/home/$(USER)/gcc-arm-none-eabi-5_4-2016q3
ARMLDFLAGS+=-L$(ARMCCLOC)/lib -L$(ARMCCLOC)/lib/gcc/arm-none-eabi/5.4.1
ARMLDFLAGS+=-L$(ARMCCLOC)/arm-none-eabi/lib/ -L$(ARMCCLOC)/lib/gcc/arm-none-eabi/10.3.1/
ARMLDFLAGS+=-L/usr/lib/arm-none-eabi/newlib/ -L/usr/lib/gcc/arm-none-eabi/10.3.1/
ARMLDFLAGS+=-lc -lgcc -lm

# Require some functions to be included from libc, for modules
ARMLDFLAGS+=-u strcasecmp -u strncasecmp -u remove -u rename -u __aeabi_i2f -u __aeabi_fmul \
-u __aeabi_fdiv -u atof -u strstr -u __aeabi_f2iz -u __errno -u __aeabi_fcmplt

SRC_FILES=src/boot.o src/bmp.o src/ui.o src/linker.o src/test.o src/ml.o src/error.o
SRC_FILES+=src/main.o src/cpu.o src/sym.o src/asm.o src/data.o src/uart.o src/app.o
FILES=$(SRC_FILES) $(EMU_FILES)

# Depend on header files
$(EMU_FILES): $(wildcard drivers/emu/*.h)
$(SRC_FILES): $(wildcard src/*.h)

os.bin: $(FILES) build-util
	$(ARMCC)-ld $(FILES) $(ARMLDFLAGS) -o os.elf
	$(ARMCC)-objcopy -O binary os.elf os.bin
	tool/frontier.o -i os.elf -o os.bin -s
	$(ARMCC)-size --format=berkeley --target=binary os.bin
