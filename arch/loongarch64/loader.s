.globl _start

_start:
	li.d	$sp, 0xA00000
	b KernelMain
