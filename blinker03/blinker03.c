
//-------------------------------------------------------------------
//-------------------------------------------------------------------

extern void PUT32 ( unsigned int, unsigned int );
extern unsigned int GET32 ( unsigned int );
extern void dummy ( void );

#define PIOA_ID 2

#define PMC_BASE  0xFFFFFC00
#define PMC_PCER  (PMC_BASE+0x0010)
#define CKGR_MOR  (PMC_BASE+0x0020)
#define CKGR_MCFR (PMC_BASE+0x0024)
#define PMC_MCKR  (PMC_BASE+0x0030)
#define PMC_SR    (PMC_BASE+0x0068)

#define PIO_BASE 0xFFFFF400
#define PIO_PER  (PIO_BASE+0x0000)
#define PIO_OER  (PIO_BASE+0x0010)
#define PIO_SODR (PIO_BASE+0x0030)
#define PIO_CODR (PIO_BASE+0x0034)

#define LED_BIT 8

#define WDT_BASE 0xFFFFFD40
#define WDT_MR (WDT_BASE+0x04)

//-------------------------------------------------------------------
static void clock_init ( void )
{
    unsigned int ra;

    //Use main oscillator directly (18.432MHz)
    PUT32(CKGR_MOR,0xFF01); //overkill, 0x701 should work
    while(1) if(GET32(PMC_SR)&(1<<0)) break;

    ra=GET32(CKGR_MCFR);
    ra&=0x1FFFF;
    if(ra==0) return; //game over no oscillator

    ra=GET32(PMC_MCKR);
    ra&=~0x00000003; //CSS
    ra|= 0x00000001; //main clock
    PUT32(PMC_MCKR,ra);
    while(1) if(GET32(PMC_SR)&(1<<3)) break;
    ra=GET32(PMC_MCKR);
    ra&=~0x0000001C; //PRES
    PUT32(PMC_MCKR,ra);
    while(1) if(GET32(PMC_SR)&(1<<3)) break;
}
//-------------------------------------------------------------------
int notmain ( void )
{
    unsigned int ra;

    PUT32(WDT_MR,1<<15); // Disable watchdog

    clock_init();

    PUT32(PMC_PCER,1<<PIOA_ID);
    PUT32(PIO_PER,1<<LED_BIT); //enable port
    PUT32(PIO_OER,1<<LED_BIT); //make output

    while(1)
    {
        PUT32(PIO_SODR,1<<LED_BIT); //write 1
        for(ra=0;ra<0x30000;ra++) dummy();
        PUT32(PIO_CODR,1<<LED_BIT); //write 0
        for(ra=0;ra<0x30000;ra++) dummy();
    }

    return(0);
}
//-------------------------------------------------------------------
//-------------------------------------------------------------------
