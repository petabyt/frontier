# Specifies data for each device
ifeq ($(ARCH),emu)
ARCH_FILES=$(patsubst %, drivers/emu/%, mem.o sys.o bmp.o io.o)
ARCH_MCPU=cortex-a5
ARCH_RUST_TARGET=armv7-unknown-linux-gnueabi
endif
