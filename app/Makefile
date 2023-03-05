ARMCC?=arm-none-eabi
RUSTC=rustc

FILES=main.o foo.o

%.o: %.c
	$(ARMCC)-gcc -c $< -o $@

RARCH=armv7-unknown-linux-gnueabi
RFLAGS=-C opt-level=2 --target $(RARCH) --emit obj --crate-type rlib
%.o: %.rs $(EXTERN_DEPS) $(wildcard *.rs)
	$(RUSTC) $(RFLAGS) $< -o $@

all: $(FILES)
	$(ARMCC)-ld -r $(FILES) -o main.elf

clean:
	$(RM) *.o *.elf
