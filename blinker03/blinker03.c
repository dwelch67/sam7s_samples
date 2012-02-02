
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------

extern void PUT32 ( unsigned int, unsigned int );
extern unsigned int GET32 ( unsigned int );
extern void dummy ( void );

#define PIOA_ID 2

#define PMC_BASE 0xFFFFFC00
#define PMC_PCER (PMC_BASE+0x0010)
#define CKGR_MOR (PMC_BASE+0x0020)
#define PMC_MCKR (PMC_BASE+0x0030)
#define PMC_SR   (PMC_BASE+0x0068)

#define PIO_BASE 0xFFFFF400
#define PIO_PER  (PIO_BASE+0x0000)
#define PIO_OER  (PIO_BASE+0x0010)
#define PIO_SODR (PIO_BASE+0x0030)
#define PIO_CODR (PIO_BASE+0x0034)

#define LED_BIT 8

#define WDT_BASE 0xFFFFFD40
#define WDT_MR (WDT_BASE+0x04)

void clock_init ( void )
{
    unsigned int ra;

    // Disable watchdog
    PUT32(WDT_MR,1<<15);
    // In case the bootloader was using the PPL lets slow down to the
    // main clock without the pll.
    ra=GET32(PMC_MCKR);
    ra&=0x1C;
    ra|=1;
    PUT32(PMC_MCKR,ra);
    while(1) if(GET32(PMC_SR)&(1<<3)) break;
    if(ra&0x1C)
    {
        ra&=0x3;
        PUT32(PMC_MCKR,ra);
        while(1) if(GET32(PMC_SR)&(1<<3)) break;
    }
}


//-------------------------------------------------------------------------
int notmain ( void )
{
    unsigned int ra;

    clock_init();

    PUT32(PMC_PCER,1<<PIOA_ID);
    PUT32(PIO_PER,1<<LED_BIT); //enable port
    PUT32(PIO_OER,1<<LED_BIT); //make output

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