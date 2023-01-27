UNICORN=/home/$(USER)/Pulled/unicorn

CFLAGS=-I$(UNICORN)/include -L$(UNICORN)/build

FILES=main.o
emulator: $(FILES)
	$(CC) $(CFLAGS) $(FILES) -larm-softmmu -lunicorn -o emulator
