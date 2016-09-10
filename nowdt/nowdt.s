
.globl _start
_start:
    ldr pc,=reset
reset:
    mov sp,#0x00200000
    add sp,sp,#0x4000
    ldr r0,=0xFFFFFD44
    mov r1,#0x8000
    ;@ str r1,[r0]
hang: b hang
