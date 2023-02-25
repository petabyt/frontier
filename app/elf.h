#include <stdint.h>

#pragma pack(push, 1)

#define ELF_32_BIT 1
#define ELF_64_BIT 2
#define LITTLE_ENDIAN 1
#define BIG_ENDIAN 2

#define RELOCATABLE 1
#define EXECUTABLE 2
#define SHARED 3
#define CORE 4

#define ELF_MAGIC 0x464C457F

struct ElfHeader32 {
	uint32_t magic;
	uint8_t bits;
	uint8_t endian;
	uint8_t version;
	uint8_t abi;
	uint8_t abi2;
	uint8_t padding[7];
	uint16_t type;
	uint16_t machine;
	uint32_t version2;
	uint32_t entry; // code entry offset
	uint32_t phoff;
	uint32_t shoff;
	uint32_t flags;
	uint16_t ehsize; // size of this header
	uint16_t phentsize; // size of program header table entry
	uint16_t phnum; // number of entries in program header table
	uint16_t shentsize; // size of header section table entry
	uint16_t shnum; // number of entries in the section header table
	uint16_t shstrndx;
};

#pragma pack(pop)
