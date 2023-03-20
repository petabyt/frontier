# Frontier
Frontier isn't a hobby OS, and isn't anywhere close to a desktop OS. It's a utility system that allows existing technology to be made more useful. Frontier can either be directly installed on bare bones systems, or on devices with an existing OS which needs more functionality.

## Philosophy
- Design an OS that will be useful in 20 years
- Use modern and futureproof technology wherever possible
- Be open to new and unfamiliar design

# Features
- 32 bit ARM v7 & v5 support (other architectures will not be considered)
- Custom graphical ARM emulator written with Unicorn Engine and Rawdraw
- Javscript Engine (mJS)
- JSON based UI Library (C based renderer)
- Support for buttons and touchscreens (not keyboards)
- 32 Bit ELF Linker for modules and apps
- 32 Bit ARM Assembler

# Design
- Apps are loaded as ELF objects, with a header including metadata (name, url, icon)
- UI structure will be stored as JSON Object created by JS - and will be parsed by C
- Apps can written in C, Rust, or Javascript (as scripts)

# Project structure
### drivers/emu
Drivers (Kernel) for the emulator
### src/
Source code for the OS - contains linker, UI renderer, etc
### mjs/
Git submodule for mJS Javascript Engine
