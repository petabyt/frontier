# Specifies data for each device
ifeq ($(ARCH),emu)
EMU_FILES=$(patsubst %, drivers/emu/%, mem.o sys.o bmp.o io.o)
ARCH_MCPU=cortex-a5
endif
