test: os.bin
	cd emulator; $(MAKE)
	emulator/emulator.o

ARMCC?=arm-none-eabi
ARMCFLAGS=-c -fpic -mcpu=cortex-a7 -fno-builtin -Idrivers/emu -Icore/ -O1 -g -Wall
ARMLDFLAGS=-Bstatic -T Linker.ld

# Add multiple locations to find GCC and libc libs
ARMCCLOC=/home/$(USER)/gcc-arm-none-eabi-5_4-2016q3
ARMLDFLAGS+=-L$(ARMCCLOC)/arm-none-eabi/lib -L$(ARMCCLOC)/lib/gcc/arm-none-eabi/5.4.1
ARMLDFLAGS+=-L/usr/lib/arm-none-eabi/newlib/ -L/usr/lib/gcc/arm-none-eabi/7.3.1/
ARMLDFLAGS+=-lc -lgcc

EMU_FILES=drivers/emu/mem.o drivers/emu/sys.o drivers/emu/bmp.o
CORE_FILES+=core/boot.o core/bmp.o drivers/emu/io.o core/ui.o core/linker.o core/main.o core/asm.o

$(EMU_FILES): $(wildcard drivers/emu/*.h)
$(CORE_FILES): $(wildcard core/*.h)

FILES=$(CORE_FILES) $(EMU_FILES)

# mJS support
FILES+=mjs/mjs.o
ARMCFLAGS+=-I. -include platform_custom.h -Imjs/ -Imjs/src
mjs/mjs.o: ARMCFLAGS+=

EXTERN_DEPS=Linker.ld

core/main.o: js.h

pack.o: pack.c
	$(CC) -g -Wall pack.c -o pack.o

os.bin: $(FILES) pack.o
	$(ARMCC)-ld $(FILES) $(ARMLDFLAGS) -o os.elf
	$(ARMCC)-ld -shared $(FILES) --strip-debug -o os2.elf
	$(ARMCC)-objcopy -O binary os.elf os.bin
	./pack.o -i os.elf -o os.bin -s
	$(ARMCC)-size --format=berkeley --target=binary os.bin
	
# output rule for C files
%.o: %.c $(EXTERN_DEPS)
	$(ARMCC)-gcc $(ARMCFLAGS) $< -o $@

# output rule for assembly files
%.o: %.S $(EXTERN_DEPS)
	$(ARMCC)-gcc $(ARMCFLAGS) $< -o $@

# output rule for rust files
RARCH=armv5te-unknown-linux-musleabi
RFLAGS=-C opt-level=2 --target $(RARCH) --emit obj --crate-type rlib
%.o: %.rs $(EXTERN_DEPS) $(wildcard *.rs)
	$(RUSTC) $(RFLAGS) $< -o $@

js.h: core/test.js core/lib.js
	xxd -i core/test.js > js.h
	xxd -i core/lib.js >> js.h

clean:
	$(RM) core/*.o drivers/emu/*.o emulator/*.o *.o *.elf *.bin js.h mjs/*.o *.out dump
