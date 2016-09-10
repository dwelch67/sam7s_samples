

.globl _start
_start:

    ;@ is this entering using flash at 0x00000000 or ram 0x00200000?
    mvn r0,#0x000000FF
    ands r0,pc
    bne skip
    mov r0,#0x00100000
    bx r0

skip:
    ldr r0,=0xFFFFFD44
    mov r1,#0x8000
    str r1,[r0]

    ldr r0,=0xFFFFFC10
    mov r1,#4
    str r1,[r0]

    ldr r0,=0xFFFFF400
    mov r1,#0x100

    str r1,[r0,#0x00]
    str r1,[r0,#0x10]

top:
    str r1,[r0,#0x30]
    mov r2,#0x300 ;@ assumes running from RC oscillator
a:
    subs r2,r2,#1
    bne a

    str r1,[r0,#0x34]
    mov r2,#0x300
b:
    subs r2,r2,#1
    bne b

    b top

hang: b hang
