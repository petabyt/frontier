-include config.mak

# Compile Unicorn VM engine and pull cnlohr rawdraw
UNICORN?=/home/$(USER)/.local/lib/python3.10/site-packages/unicorn
RAWDRAW?=$(shell echo ../../rawdraw)

CFLAGS?=-I$(UNICORN)/include -L$(UNICORN)/lib -I$(RAWDRAW) -I../drivers/emu/

FILES=main.o
emulator.o: $(FILES)
	$(CC) $(CFLAGS) $(FILES) -lunicorn -lm -lX11 -o emulator.o
