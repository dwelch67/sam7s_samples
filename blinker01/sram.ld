
MEMORY
{
    ram : ORIGIN = 0x00200000, LENGTH = 16K
}

SECTIONS
{
    .text : { *(.text*) } > ram
    .bss : { *(.bss*) } > ram
}

