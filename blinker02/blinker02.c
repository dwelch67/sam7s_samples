
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------

extern void PUT32 ( unsigned int, unsigned int );
extern void dummy ( void );

#define PIOA_ID 2

#define PMC_BASE 0xFFFFFC00
#define PMC_PCER (PMC_BASE+0x0010)

#define PIO_BASE 0xFFFFF400
#define PIO_PER  (PIO_BASE+0x0000)
#define PIO_OER  (PIO_BASE+0x0010)
#define PIO_SODR (PIO_BASE+0x0030)
#define PIO_CODR (PIO_BASE+0x0034)

#define LED_BIT 8

//-------------------------------------------------------------------------
int notmain ( void )
{
    unsigned int ra;

    PUT32(PMC_PCER,1<<PIOA_ID);
    PUT32(PIO_PER,1<<LED_BIT); //enable port
    PUT32(PIO_PER,1<<LED_BIT); //make output
    while(1)
    {
        PUT32(PIO_CODR,1<<LED_BIT); //write 0 led on
        for(ra=0;ra<0x30000;ra++) dummy();
        PUT32(PIO_SODR,1<<LED_BIT); //write 0 led off
        for(ra=0;ra<0x30000;ra++) dummy();
    }

    return(0);
}
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
