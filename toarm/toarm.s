
.thumb

.globl _start
_start:
    ldr r0,=arm_start
    bx r0

.code 32
.align

arm_start:
    nop
    nop
    nop
    b arm_start

