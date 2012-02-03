
.globl _start
_start:
    mov sp,#0x00200000
    add sp,sp,#0x4000
    bl notmain
hang: b hang

.globl PUT32
PUT32:
    str r1,[r0]
    bx lr

.globl GET32
GET32:
    ldr r0,[r0]
    bx lr

.globl ASMDELAY
ASMDELAY:
    subs r0,#1
    bne ASMDELAY
    bx lr

.globl doexit
doexit:
    b doexit
