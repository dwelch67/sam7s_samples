
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------

extern void PUT32 ( unsigned int, unsigned int );
extern unsigned int GET32 ( unsigned int );
extern void dummy ( void );

#define PIOA_ID 2
#define TC0_ID  12

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

#define TC_BASE 0xFFFA0000
#define TC_BMR (TC_BASE+0xC4)
#define TC0_CCR (TC_BASE+0x00+0x00)
#define TC0_CMR (TC_BASE+0x00+0x04)
#define TC0_CV  (TC_BASE+0x00+0x10)
#define TC0_IDR (TC_BASE+0x00+0x28)

#define TC1_CCR (TC_BASE+0x40+0x00)
#define TC2_CCR (TC_BASE+0x80+0x00)

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
void dowait ( void )
{
    //unsigned int ra;

    //for(ra=0;ra<100;ra++)
    {
        while((GET32(TC0_CV)&0x8000)==0x0000) continue;
        while((GET32(TC0_CV)&0x8000)==0x8000) continue;
    }
}
//-------------------------------------------------------------------------
int notmain ( void )
{
    unsigned int ra;

    clock_init();

    PUT32(PMC_PCER,1<<TC0_ID);
    PUT32(TC_BMR,1);
    PUT32(TC0_CMR,4);
    PUT32(TC0_IDR,0xFF);
    PUT32(TC0_CCR,5);

    PUT32(PMC_PCER,1<<PIOA_ID);
    PUT32(PIO_PER,1<<LED_BIT); //enable port
    PUT32(PIO_OER,1<<LED_BIT); //make output

    while(1)
    {
        PUT32(PIO_CODR,1<<LED_BIT); //write 0 led on
        dowait();
        //for(ra=0;ra<0x30000;ra++) dummy();
        PUT32(PIO_SODR,1<<LED_BIT); //write 0 led off
        dowait();
        //for(ra=0;ra<0x30000;ra++) dummy();
    }

    //starting with 18432000 Hz
    //using 18432000/1024 = 18000
    //16 bit counter 65536/18000 = 3.641 seconds
    //led should change state somewhere between 3 and 4 seconds, which
    //it does, so we are using the 18432000 hz clock

    return(0);
}
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
