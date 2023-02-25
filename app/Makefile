ARMCC?=arm-none-eabi

all:
	$(ARMCC)-gcc -c main.c -o main.o
	$(ARMCC)-ld -shared main.o -o main.elf
	tcc -run elf.c
