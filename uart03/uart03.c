
//-------------------------------------------------------------------
//-------------------------------------------------------------------

//PA5 RXD0  PA6 TXD0

extern void PUT32 ( unsigned int, unsigned int );
extern unsigned int GET32 ( unsigned int );
extern void dummy ( void );

#define PIOA_ID 2
#define TC0_ID  12
#define US0_ID  6
#define US1_ID  7

#define PMC_BASE  0xFFFFFC00
#define PMC_PCER  (PMC_BASE+0x0010)
#define CKGR_MOR  (PMC_BASE+0x0020)
#define CKGR_MCFR (PMC_BASE+0x0024)
#define CKGR_PLLR (PMC_BASE+0x002C)
#define PMC_MCKR  (PMC_BASE+0x0030)
#define PMC_SR    (PMC_BASE+0x0068)

#define PIO_BASE 0xFFFFF400
#define PIO_PER  (PIO_BASE+0x0000)
#define PIO_PDR  (PIO_BASE+0x0004)
#define PIO_OER  (PIO_BASE+0x0010)
#define PIO_SODR (PIO_BASE+0x0030)
#define PIO_CODR (PIO_BASE+0x0034)
#define PIO_ASR  (PIO_BASE+0x0070)

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

#define UART0_BASE 0xFFFC0000

#define US0_CR   (UART0_BASE+0x0000)
#define US0_MR   (UART0_BASE+0x0004)
#define US0_IER  (UART0_BASE+0x0008)
#define US0_IDR  (UART0_BASE+0x000C)
#define US0_IMR  (UART0_BASE+0x0010)
#define US0_CSR  (UART0_BASE+0x0014)
#define US0_RHR  (UART0_BASE+0x0018)
#define US0_THR  (UART0_BASE+0x001C)
#define US0_BRGR (UART0_BASE+0x0020)
#define US0_RTOR (UART0_BASE+0x0024)
#define US0_TTGR (UART0_BASE+0x0028)
#define US0_FIDI (UART0_BASE+0x0174)
#define US0_NER  (UART0_BASE+0x0044)
#define US0_IF   (UART0_BASE+0x004C)

#define EFC_BASE 0xFFFFFF00
#define MC_FMR (EFC_BASE+0x60)
#define MC_FCR (EFC_BASE+0x64)
#define MC_FSR (EFC_BASE+0x68)


//-------------------------------------------------------------------
static void clock_init ( void )
{
    unsigned int ra;

    //use main oscillator and PLL to get close to 48MHz.

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

    //Someday need/want 48MHz for USB.  PLL clock needs to be
    //between 80 and 160MHz.  48*2 = 96  48*4 = 192
    //96/18.432 = 5.2083
    //5.2083 * 5 =  26.04
    //some examples you see divide by 5 and multiply by 26
    //to get 95.846
    //PUT32(CKGR_PLLR,0x10193F05);
    //Even an Atmel document does something like that but, if you
    //try to find the minimum error (pllmath.c)
    //18.432/24 * 125 = 96Mhz exactly.
    //PUT32(CKGR_PLLR,0x107C3F18);
    //It says Fin is between 1 and 32MHz, is Fin before the divide
    //or after the divide?
    //18.432/14 * 73 = 96109  closer than the /5*26
    PUT32(CKGR_PLLR,0x10483F0E);

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
static void uart_init ( void )
{
    PUT32(PMC_PCER,1<<PIOA_ID);

    PUT32(PMC_PCER,1<<US0_ID);
    //PA5 RXD0  PA6 TXD0
    PUT32(PIO_PDR,(1<<5)|(1<<6));
    PUT32(PIO_ASR,(1<<5)|(1<<6));

    PUT32(US0_CR,(1<<2)|(1<<3)|(1<<5)|(1<<7)); //reset and disable rx and tx

    PUT32(US0_MR,(3<<6)|(4<<9));
    PUT32(US0_IDR,0xFFFFFFFF);
    //starting with 18432000 Hz
    //18432000*73/28 = 48054857
    //48054857/(16*115200) = 26.07
    PUT32(US0_BRGR,26);
    PUT32(US0_RTOR,0);
    PUT32(US0_TTGR,0);
    PUT32(US0_FIDI,0);
    PUT32(US0_IF,0);
    PUT32(US0_CR,(1<<6)|(1<<4)); //enable TX and RX
}
//-------------------------------------------------------------------
static void uart_putc ( unsigned int c )
{
    while(1) if(GET32(US0_CSR)&(1<<9)) break;
    PUT32(US0_THR,c);
}
//-------------------------------------------------------------------
static unsigned int uart_getc ( void  )
{
    while(1) if(GET32(US0_CSR)&(1<<0)) break;
    return(GET32(US0_RHR));
}
//-------------------------------------------------------------------
void hexstrings ( unsigned int d )
{
    unsigned int rb;
    unsigned int rc;

    rb=32;
    while(1)
    {
        rb-=4;
        rc=(d>>rb)&0xF;
        if(rc>9) rc+=0x37; else rc+=0x30;
        uart_putc(rc);
        if(rb==0) break;
    }
    uart_putc(0x20);
}
//-------------------------------------------------------------------
void hexstring ( unsigned int d )
{
    hexstrings(d);
    uart_putc(0x0D);
    uart_putc(0x0A);
}
//-------------------------------------------------------------------
int notmain ( void )
{
    unsigned int ra;

    PUT32(WDT_MR,1<<15); // Disable watchdog

    clock_init();
    uart_init();

    hexstring(0x12345678);
    hexstring(0x87654321);

    while(1)
    {
        ra=uart_getc();
        uart_putc(ra);
        if(ra==0x0D) uart_putc(0x0A);
    }


    return(0);
}
//-------------------------------------------------------------------
//-------------------------------------------------------------------
