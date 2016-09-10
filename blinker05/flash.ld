
MEMORY
{
    ram : ORIGIN = 0x00200000, LENGTH = 16K
    rom : ORIGIN = 0x00100000, LENGTH = 16K
}

SECTIONS
{
    .text : { *(.text*) } > rom
    .bss : { *(.bss*) } > ram
}

