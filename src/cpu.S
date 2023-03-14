.global asm_get_cpuinfo
asm_get_cpuinfo:
	push {lr}

	mrc p15, 0x0, r0, cr0, cr0, 0x0

	pop {pc}

.global asm_exec
asm_exec:
	push {lr}

	blx r0

	pop {pc}
