# Common portable makefile things
ARMCC?=arm-none-eabi
RUSTC?=rustc
ARMCFLAGS?=-c -mcpu=$(ARCH_MCPU) -fno-builtin -Idrivers/$(ARCH)/ -I. -Isrc/ -g -Wall -O2

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
	$(RM) $(TOPL)/src/*.o $(TOPL)/drivers/emu/*.o $(TOPL)/emulator/*.o *.o *.elf *.bin *.out $(TOPL)/dump

build-tool:
	cd tool && $(MAKE)

build-emu:
	cd emulator && $(MAKE)
