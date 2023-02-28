-include config.mak

# Compile Unicorn VM engine and pull cnlohr rawdraw
UNICORN?=/home/$(USER)/Pulled/unicorn
RAWDRAW?=$(shell echo ../../rawdraw)

CFLAGS?=-I$(UNICORN)/include -L$(UNICORN)/build -I$(RAWDRAW) -I../drivers/emu/

FILES=main.o
emulator.o: $(FILES)
	$(CC) $(CFLAGS) $(FILES) -larm-softmmu -lunicorn -lX11 -o emulator.o
