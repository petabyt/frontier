NAME=tetris
all: $(NAME).elf
TOPL?=../../
include $(TOPL)/common.mk

ARMCFLAGS=-c -I$(TOPL)/src/ -O4 -mcpu=cortex-a7
FILES=main.o

$(NAME).elf: $(FILES) ptetris.h
	$(ARMCC)-ld -r $(FILES) -o $(NAME).elf
