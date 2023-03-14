test: os.bin
	cd app; $(MAKE)
	cd emulator; $(MAKE)
	emulator/emulator.o

ARMCC?=arm-none-eabi
ARMCFLAGS=-c -mcpu=cortex-a7 -fno-builtin -Idrivers/emu -Isrc/ -O1 -g -Wall
ARMLDFLAGS=-T Linker.ld

# Add multiple locations to find GCC and libc libs
ARMCCLOC=/home/$(USER)/gcc-arm-none-eabi-5_4-2016q3
ARMLDFLAGS+=-L$(ARMCCLOC)/arm-none-eabi/lib -L$(ARMCCLOC)/lib/gcc/arm-none-eabi/5.4.1
ARMLDFLAGS+=-L/usr/lib/arm-none-eabi/newlib/ -L/usr/lib/gcc/arm-none-eabi/10.3.1/
ARMLDFLAGS+=-lc -lgcc -lm
#ARMLDFLAGS+=-L/home/daniel/Pulled/musl/lib
#ARMLDFLAGS+=-L/home/daniel/Pulled/musl/lib
#ARMLDFLAGS+=-lc
#ARMLDFLAGS+=/home/daniel/Pulled/musl/obj/src/string/memset.lo

ARMLDFLAGS+=-u strcasecmp -u strncasecmp -u remove -u rename -u __aeabi_i2f -u __aeabi_fmul \
-u __aeabi_fdiv -u atof -u strstr -u __aeabi_f2iz

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

EXTERN_DEPS=Linker.ld

src/main.o: js.h

fpack.o: fpack.c
	$(CC) -g -Wall fpack.c -o fpack.o

os.bin: $(FILES) fpack.o Makefile
	$(ARMCC)-ld $(FILES) $(ARMLDFLAGS) -o os.elf
	$(ARMCC)-objcopy -O binary os.elf os.bin
	./fpack.o -i os.elf -o os.bin -s
	$(ARMCC)-size --format=berkeley --target=binary os.bin
	
# output rule for C files
%.o: %.c $(EXTERN_DEPS)
	$(ARMCC)-gcc $(ARMCFLAGS) $< -o $@

# output rule for assembly files
%.o: %.S $(EXTERN_DEPS)
	$(ARMCC)-gcc $(ARMCFLAGS) $< -o $@
%.o: %.s $(EXTERN_DEPS)
	$(ARMCC)-gcc $(ARMCFLAGS) $< -o $@

# output rule for rust files
RARCH=armv7-unknown-linux-gnueabi
RFLAGS=-C opt-level=2 --target $(RARCH) --emit obj --crate-type rlib
%.o: %.rs $(EXTERN_DEPS) $(wildcard *.rs)
	$(RUSTC) $(RFLAGS) $< -o $@

clean:
	$(RM) src/*.o drivers/emu/*.o emulator/*.o *.o *.elf *.bin js.h *.out dump

