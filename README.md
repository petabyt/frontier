# FrontierOS
What will the world look like in 20 years? What technology will we be using? What OS will we use?  
Frontier is a utility operating system that explores the next generation of hacking - in a world  
of prosperity, where humans inhabit other planets.

Frontier isn't a hobby OS, and isn't anywhere close to a desktop OS. It's a utility system that allows existing technology to be made more useful. Frontier can either be directly installed on bare bones systems, or on systems with an existing OS which needs more functionality.

## Philosophy
- Design an OS that will be useful in 20 years
- Use modern and futureproof technology wherever possible
- Be open to new and unfamiliar design

# Design
- 32 bit ARM v7 & v5 support (other architectures will not be considered)
- Custom ARM emulator (Unicorn VM) and basic 'kernel'
- JSON based UI Library - can be modified by JavaScript - structure is read and rendered by C
- UI lib will support buttons and touchscreen
- Apps are loaded as ELF objects, with a header including metadata (name, url, icon)
