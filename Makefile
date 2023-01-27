test: os.bin
	cd emulator; $(MAKE)
	emulator/emulator

ARMCFLAGS=-c -fpack-struct -fpic -mcpu=cortex-a7
ARMCC?=arm-none-eabi
FILES=core/boot.o core/main.o core/lib.o
ARMLDFLAGS=-Bstatic -T Linker.ld
os.bin: $(FILES)
	$(ARMCC)-ld $(ARMLDFLAGS) $(FILES) -o os.elf
	$(ARMCC)-objcopy -O binary os.elf os.bin

# output rule for C files
%.o: %.c $(EXTERN_DEPS)
	$(ARMCC)-gcc $(ARMCFLAGS) $< -o $@

# output rule for assembly files
%.o: %.S $(EXTERN_DEPS)
	$(ARMCC)-gcc $(ARMCFLAGS) $< -o $@

clean:
	$(RM) core/*.o drivers/emu/*.o emulator/emulator *.o *.elf *.bin
