ARMCC?=arm-none-eabi
RUSTC?=rustc

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
	$(RM) src/*.o drivers/emu/*.o emulator/*.o *.o *.elf *.bin *.out dump

