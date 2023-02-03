test: os.bin
	@cd emulator; $(MAKE)
	emulator/emulator.o

ARMCFLAGS=-c -fpic -mcpu=cortex-a7 -fno-builtin -Idrivers/emu -Icore/ -O1 -g
ARMCC?=arm-none-eabi
ARMLDFLAGS=-Bstatic -T Linker.ld

# Add multiple locations to find GCC and libc libs
ARMCCLOC=/home/$(USER)/gcc-arm-none-eabi-5_4-2016q3
ARMLDFLAGS+=-L$(ARMCCLOC)/arm-none-eabi/lib -L$(ARMCCLOC)/lib/gcc/arm-none-eabi/5.4.1
ARMLDFLAGS+=-L/usr/lib/arm-none-eabi/newlib/ -L/usr/lib/gcc/arm-none-eabi/7.3.1/
ARMLDFLAGS+=-lc -lgcc

FILES=core/boot.o core/main.o core/asm.o drivers/emu/mem.o drivers/emu/sys.o drivers/emu/bmp.o
FILES+=core/bmp.o drivers/emu/io.o core/ui.o

# mJS support
FILES+=mjs/mjs.o
ARMCFLAGS+=-I. -include platform_custom.h -Imjs/ -Imjs/src
mjs/mjs.o: ARMCFLAGS+=

EXTERN_DEPS=Makefile Linker.ld

core/main.o: js.h

sym.o: sym.c
	$(CC) sym.c -o sym.o

os.bin: $(FILES) sym.o
	$(ARMCC)-ld $(FILES) $(ARMLDFLAGS) -o os.elf
	$(ARMCC)-objcopy -O binary os.elf os.bin
	./sym.o
	dd if=symtbl.bin bs=1G >> os.bin
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
