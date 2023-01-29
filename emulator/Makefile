-include config.mak

UNICORN=/home/$(USER)/Pulled/unicorn

# Compile Unicorn VM engine and pull cnlohr rawdraw
CFLAGS?=-I$(UNICORN)/include -L$(UNICORN)/build -I/home/$(USER)/Documents/rawdraw

FILES=main.o
emulator.o: $(FILES)
	$(CC) $(CFLAGS) $(FILES) -larm-softmmu -lunicorn -lX11 -o emulator.o
