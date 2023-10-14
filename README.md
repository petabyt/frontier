# Frontier
Frontier is a bare metal ARM OS designed to run on consumer hardware.

![PoC](https://eggnog.danielc.dev/f/74-finmvdwilqibugm0au5wi02i9g6427.png)

It consists of:
- Basic monolithic kernel with module support
- 32 Bit ELF loader for modules and apps
- Embedded ARM Assembler and memory patch manager
- Javscript Engine (mJS)
- Basic JSON-based UI Library (native rendering)
- Support for buttons and touchscreens (not keyboards)
- Generic graphical ARM emulator written with Unicorn Engine

# Design
- Priority support for 32 bit ARM v6. 64 bit and Thumb is secondary, other archs will not be considered.
- Aiming for a 100k standalone ARM32 image
- POSIX interface when possible, other graphics and IO functions are very basic
- Apps are standard ELF objects, with an optional header including metadata (name, url, icon)
- Magic Lantern modules can also be loaded, with a basic compatibility layer
- UI structure will be stored as JSON Object created by JS - and will be parsed by C
- Apps can be written in C, Rust, or Javascript (as scripts)

## Philosophy
- Design an OS that will be useful in 20 years
- Use modern and futureproof technology wherever possible

# Roadmap
- [x] Basic emulator, kernel, hello world tests
- [x] Link and test libc, libm, libgcc
- [x] ELF Loader implemented
- [x] Basic UI Library
- [ ] Multithreading in emulator
- [ ] Port to generic qemu
- [ ] Basic shell for debugging
- [ ] Port for thumb bytecode (test on micro:bit)
- [x] Button & mouse IO

# Project structure
### drivers/emu
Drivers (Kernel) for the emulator
### src/
Source code for the OS - contains loader, UI renderer, etc
### apps/
Contains all the basic test apps - and the mJS engine
