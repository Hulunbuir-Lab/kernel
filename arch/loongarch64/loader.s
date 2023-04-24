.globl _start

_start:
	li.d	$t0, 0x9000000000000001
	csrwr	$t0, 0x181
	li.d	$t0, 0xF
	csrwr	$t0, 0x180
	csrwr	$zero, 0x182
	csrwr	$zero, 0x183
	li.d	$t0, 0x8
	csrwr	$t0, 0x0
	la.local	$t1, KernelMain
	li.d	$t0, 0xFFFFFFF
	and		$t1, $t1, $t0
	jr $t1
