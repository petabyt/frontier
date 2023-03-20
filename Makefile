test: os.bin
	cd hello; $(MAKE)
	emulator/frontier.o -i os.bin -e

TOPL=.
-include common.mk

ARMCC?=arm-none-eabi
ARMCFLAGS=-c -mcpu=cortex-a7 -fno-builtin -Idrivers/emu -Isrc/ -g -Wall -O4
ARMLDFLAGS=-T Linker.ld

# Add multiple locations to find GCC and libc libs
ARMCCLOC=/home/$(USER)/gcc-arm-none-eabi-5_4-2016q3
ARMLDFLAGS+=-L$(ARMCCLOC)/arm-none-eabi/lib -L$(ARMCCLOC)/lib/gcc/arm-none-eabi/5.4.1
ARMLDFLAGS+=-L/usr/lib/arm-none-eabi/newlib/ -L/usr/lib/gcc/arm-none-eabi/10.3.1/
ARMLDFLAGS+=-lc -lgcc -lm

ARMLDFLAGS+=-u strcasecmp -u strncasecmp -u remove -u rename -u __aeabi_i2f -u __aeabi_fmul \
-u __aeabi_fdiv -u atof -u strstr -u __aeabi_f2iz -u __errno

EMU_FILES=drivers/emu/mem.o drivers/emu/sys.o drivers/emu/bmp.o drivers/emu/io.o
SRC_FILES=src/boot.o src/bmp.o src/ui.o src/linker.o src/test.o
SRC_FILES+=src/main.o src/cpu.o src/sym.o src/asm.o src/data.o
FILES=$(SRC_FILES) $(EMU_FILES)

# Depend on header files
$(EMU_FILES): $(wildcard drivers/emu/*.h)
$(SRC_FILES): $(wildcard src/*.h)

# mJS support
FILES+=mjs/mjs.o
ARMCFLAGS+=-I. -include platform_custom.h -Imjs/ -Imjs/src

build-util:
	cd emulator; $(MAKE)

os.bin: $(FILES) build-util Makefile Linker.ld
	$(ARMCC)-ld $(FILES) $(ARMLDFLAGS) -o os.elf
	$(ARMCC)-objcopy -O binary os.elf os.bin
	emulator/frontier.o -i os.elf -o os.bin -s
	$(ARMCC)-size --format=berkeley --target=binary os.bin
