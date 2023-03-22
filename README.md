# Frontier
Frontier is a utility operating system that allows existing technology to be made more useful. Frontier can either be directly installed on bare bones systems, or on devices with an existing OS which needs more functionality. The final product will be comparable to a mobile OS such as Android, but operating on less than a megabyte of RAM.

## Philosophy
- Design an OS that will be useful in 20 years
- Use modern and futureproof technology wherever possible

# Features
- Basic monolithic kernel with module support
- Priority 32 bit ARM v7 & v5 support (other architectures will not be considered)
- Custom graphical ARM emulator written with Unicorn Engine and Rawdraw
- Javscript Engine (mJS)
- JSON based UI Library (C based renderer)
- Support for buttons and touchscreens (not keyboards)
- 32 Bit ELF Linker for modules and apps
- Runtime ARM Assembler

# Design
- Apps are loaded as ELF objects, with a header including metadata (name, url, icon)
- Apps without metadata can be detected and loaded, as well as Magic Lantern modules
- UI structure will be stored as JSON Object created by JS - and will be parsed by C
- Apps can written in C, Rust, or Javascript (as scripts)

# Roadmap
- [x] Basic emulator, kernel, hello world tests
- [x] Linker implemented
- [ ] UI Library

# Project structure
### drivers/emu
Drivers (Kernel) for the emulator
### src/
Source code for the OS - contains linker, UI renderer, etc
### mjs/
Git submodule for mJS Javascript Engine
