NAME=js
all: $(NAME).elf
TOPL?=../../
include $(TOPL)/common.mk

#FILES=rust.o

$(NAME).elf: $(FILES)
	$(ARMCC)-ld -r $(FILES) -o $(NAME).elf
