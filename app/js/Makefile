NAME=js
all: $(NAME).elf
TOPL?=../../
include $(TOPL)/common.mak

FILES=mjs.o

$(NAME).elf: $(FILES)
	$(ARMCC)-ld -r $(FILES) -o $(NAME).elf
