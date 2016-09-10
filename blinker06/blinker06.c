
//-------------------------------------------------------------------
//-------------------------------------------------------------------

extern void PUT32 ( unsigned int, unsigned int );
extern unsigned int GET32 ( unsigned int );
extern void dummy ( void );

#define PIOA_ID 2
#define TC0_ID  12

#define PMC_BASE  0xFFFFFC00
#define PMC_PCER  (PMC_BASE+0x0010)
#define CKGR_MOR  (PMC_BASE+0x0020)
#define CKGR_MCFR (PMC_BASE+0x0024)
#define CKGR_PLLR (PMC_BASE+0x002C)
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

#define TC_BASE 0xFFFA0000
#define TC_BMR (TC_BASE+0xC4)
#define TC0_CCR (TC_BASE+0x00+0x00)
#define TC0_CMR (TC_BASE+0x00+0x04)
#define TC0_CV  (TC_BASE+0x00+0x10)
#define TC0_IDR (TC_BASE+0x00+0x28)

#define TC1_CCR (TC_BASE+0x40+0x00)
#define TC2_CCR (TC_BASE+0x80+0x00)

#define EFC_BASE 0xFFFFFF00
#define MC_FMR (EFC_BASE+0x60)
#define MC_FCR (EFC_BASE+0x64)
#define MC_FSR (EFC_BASE+0x68)

//-------------------------------------------------------------------
static void clock_init ( void )
{
    unsigned int ra;

    //use main oscillator and PLL to get close to 55MHz.

    //force slow RC oscillator
    ra=GET32(PMC_MCKR);
    ra&=~0x00000003; //CSS
    PUT32(PMC_MCKR,ra);
    while(1) if(GET32(PMC_SR)&(1<<3)) break;
    ra=GET32(PMC_MCKR);
    ra&=~0x0000001C; //PRES
    PUT32(PMC_MCKR,ra);
    while(1) if(GET32(PMC_SR)&(1<<3)) break;

    //start main
    PUT32(CKGR_MOR,0xFF01); //overkill, 0x701 should work
    while(1) if(GET32(PMC_SR)&(1<<0)) break;

    ra=GET32(CKGR_MCFR);
    ra&=0x1FFFF;
    if(ra==0) return; //game over no oscillator

    //have to slow flash
    PUT32(MC_FMR,0x00000100);

    //d 187 m 1116 110000 0
    //dont know if Fin is main clock in or main clock divided

    //d 18 m 107 109568 432
    PUT32(CKGR_PLLR,0x206A3F12);

    while(1) if(GET32(PMC_SR)&(1<<2)) break;
    while(1) if(GET32(PMC_SR)&(1<<3)) break;

    ra=GET32(PMC_MCKR);
    //ra&=~0x0000001C; //PRES did this above
    ra|= 0x00000004; //divide by 2
    PUT32(PMC_MCKR,ra);
    while(1) if(GET32(PMC_SR)&(1<<3)) break;
    ra=GET32(PMC_MCKR);
    //ra&=~0x00000003; //CSS
    ra|= 0x00000003; //main clock
    PUT32(PMC_MCKR,ra);
    while(1) if(GET32(PMC_SR)&(1<<3)) break;
}
//-------------------------------------------------------------------
static void dowait ( void )
{
    unsigned int ra;

//    for(ra=0;ra<50;ra++) //61 seconds
    for(ra=0;ra<10;ra++) //12.5
    {
        while((GET32(TC0_CV)&0x8000)==0x0000) continue;
        while((GET32(TC0_CV)&0x8000)==0x8000) continue;
    }
}
//-------------------------------------------------------------------
int notmain ( void )
{
    PUT32(WDT_MR,1<<15);  // Disable watchdog

    clock_init();

    PUT32(PMC_PCER,1<<TC0_ID);
    PUT32(TC0_CMR,4);
    PUT32(TC0_IDR,0xFF);
    PUT32(TC0_CCR,5);

    PUT32(PMC_PCER,1<<PIOA_ID);
    PUT32(PIO_PER,1<<LED_BIT); //enable port
    PUT32(PIO_OER,1<<LED_BIT); //make output

    while(1)
    {
        PUT32(PIO_SODR,1<<LED_BIT); //write 1
        dowait();
        PUT32(PIO_CODR,1<<LED_BIT); //write 0
        dowait();
    }

    //starting with 18432000 Hz
    //18432000*107/36 = 54784000
    //54784000/1024 = 53500
    //(65536*10) / 53500 = 12.25 seconds (or *50 = 61 seconds)
    //led should change state every 12.25 seconds, which
    //it does, so we are using the 54.784 MHz clock

    return(0);
}
//-------------------------------------------------------------------
//-------------------------------------------------------------------
