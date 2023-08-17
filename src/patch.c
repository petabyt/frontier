// Copyright (C) 2022-2023 Frontier by Daniel C - Apache License
// Completely unfinished
#include <stdint.h>

enum Patches {
	PATCH_U8,
	PATCH_U16,
	PATCH_U32,
	PATCH_U64,
	PATCH_MEM,
	PATCH_JUMP,
	PATCH_CALL,
};

struct Patch {
	uintptr_t address;
	int type;
	int length;
	char data[];
};
