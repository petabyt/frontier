.global asm_get_cpuinfo
asm_get_cpuinfo:
	push {lr}

	mrc p15, 0x0, r0, cr0, cr0, 0x0

	pop {pc}

.extern uart_log
.global printf
printf:
	b uart_log
