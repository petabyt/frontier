all: os.bin build-emu
	emulator/emu.o -i os.bin -e

-include $(TOPL)/config.mak
ARCH?=emu
include arch.mak

TOPL=.
include common.mak

ARMLDFLAGS=-T Linker.ld

# Add multiple locations to find GCC and libc libs
ARMCCLOC?=/home/$(USER)/gcc-arm-none-eabi-5_4-2016q3
ARMLDFLAGS+=-L$(ARMCCLOC)/lib -L$(ARMCCLOC)/lib/gcc/arm-none-eabi/5.4.1
ARMLDFLAGS+=-L$(ARMCCLOC)/arm-none-eabi/lib/ -L$(ARMCCLOC)/lib/gcc/arm-none-eabi/10.3.1/
ARMLDFLAGS+=-L/usr/lib/arm-none-eabi/newlib/ -L/usr/lib/gcc/arm-none-eabi/10.3.1/
ARMLDFLAGS+=-lc -lgcc -lm

SRC_FILES=boot.o bmp.o ui.o linker.o test.o ml.o error.o main.o cpu.o sym.o asm.o data.o uart.o app.o
FILES=$(addprefix src/,$(SRC_FILES)) $(ARCH_FILES)

# Depend on header files
$(FILES): $(wildcard src/*.h)

os.bin: $(FILES) build-tool
	$(ARMCC)-ld $(FILES) $(ARMLDFLAGS) -o os.elf
	$(ARMCC)-objcopy -O binary os.elf os.bin
	tool/frontier.o -i os.elf -o os.bin -s
	$(ARMCC)-size --format=berkeley --target=binary os.bin
