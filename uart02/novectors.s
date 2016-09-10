
.globl _start
_start:

    ;@ is this entering using flash at 0x00000000 or ram 0x00200000?
    mvn r0,#0x000000FF
    ands r0,pc
    bne skip
    mov r0,#0x00100000
    bx r0

skip:

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

.globl dummy
dummy:
    bx lr
