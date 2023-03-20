ARMCC?=arm-none-eabi
RUSTC=rustc

FILES=mandelbrot.o

CFLAGS=-c -I../src/ -O4 -mcpu=cortex-a7

all: $(FILES)
	$(ARMCC)-ld -r $(FILES) -o main.elf

%.o: %.c ptetris.h Makefile
	$(ARMCC)-gcc $(CFLAGS) $< -o $@

RARCH=armv7-unknown-linux-gnueabi
RFLAGS=-C opt-level=2 --target $(RARCH) --emit obj --crate-type rlib
%.o: %.rs $(EXTERN_DEPS) $(wildcard *.rs)
	$(RUSTC) $(RFLAGS) $< -o $@

clean:
	$(RM) *.o *.elf
