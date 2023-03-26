test: os.bin
	emulator/frontier.o -i os.bin -e

TOPL=.
-include common.mk
-include $(TOPL)/config.mak

# Should be specified in config.mak
ARCH?=emu
INTERFACE?=headless

ARMCC?=arm-none-eabi
ARMCFLAGS=-c -mcpu=cortex-a5 -fno-builtin -Idrivers/$(ARCH)/ -I. -Isrc/ -g -Wall -O2
ARMLDFLAGS=-T Linker.ld

# Add multiple locations to find GCC and libc libs
ARMCCLOC?=/home/$(USER)/gcc-arm-none-eabi-5_4-2016q3
ARMLDFLAGS+=-L$(ARMCCLOC)/arm-none-eabi/lib/ -L$(ARMCCLOC)/lib/gcc/arm-none-eabi/10.3.1/
ARMLDFLAGS+=-L/usr/lib/arm-none-eabi/newlib/ -L/usr/lib/gcc/arm-none-eabi/10.3.1/
ARMLDFLAGS+=-lc -lgcc -lm

# Require some functions to be included from libc
ARMLDFLAGS+=-u strcasecmp -u strncasecmp -u remove -u rename -u __aeabi_i2f -u __aeabi_fmul \
-u __aeabi_fdiv -u atof -u strstr -u __aeabi_f2iz -u __errno

ifeq ($(ARCH),emu)
EMU_FILES=$(patsubst %, drivers/emu/%, mem.o sys.o bmp.o io.o)
endif

SRC_FILES=src/boot.o src/bmp.o src/ui.o src/linker.o src/test.o
SRC_FILES+=src/main.o src/cpu.o src/sym.o src/asm.o src/data.o src/uart.o src/js.o
FILES=$(SRC_FILES) $(EMU_FILES)

# Depend on header files
$(EMU_FILES): $(wildcard drivers/emu/*.h)
$(SRC_FILES): $(wildcard src/*.h)

# mJS support
FILES+=mjs/mjs.o
ARMCFLAGS+=-Imjs/ -Imjs/src

build-util:
	cd emulator && $(MAKE)

os.bin: $(FILES) build-util
	$(ARMCC)-ld $(FILES) $(ARMLDFLAGS) -o os.elf
	$(ARMCC)-objcopy -O binary os.elf os.bin
	emulator/frontier.o -i os.elf -o os.bin -s
	$(ARMCC)-size --format=berkeley --target=binary os.bin
