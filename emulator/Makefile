-include config.mak

# Compile Unicorn VM engine and pull cnlohr rawdraw
UNICORN?=/home/$(USER)/Pulled/unicorn
RAWDRAW?=$(shell echo ../../rawdraw)

CFLAGS?=-I$(RAWDRAW) -I../drivers/emu/ -O4

FILES=main.o
emulator.o: $(FILES) Makefile
	$(CC) $(CFLAGS) $(FILES) -lunicorn -lX11 -lGL -lm -o emulator.o
