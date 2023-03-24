NAME=hello
all: $(NAME).elf
TOPL?=../../
include $(TOPL)/common.mk

FILES=rust.o
#ARMCFLAGS=-c -I$(F)/src/ -O4 -mcpu=cortex-a7

$(NAME).elf: $(FILES)
	$(ARMCC)-ld -r $(FILES) -o $(NAME).elf
