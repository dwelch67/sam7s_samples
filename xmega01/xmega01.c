
#include "blinker.h"

#include "parity.h"


extern void PUT32 ( unsigned int, unsigned int );
extern unsigned int GET32 ( unsigned int );
extern void dummy ( void );
extern void doexit ( unsigned int );
extern void ASMDELAY ( unsigned int );

#define PIOA_ID 2
#define TC0_ID  12
#define US0_ID  6
#define US1_ID  7

#define PMC_BASE 0xFFFFFC00
#define PMC_PCER (PMC_BASE+0x0010)
#define CKGR_MOR (PMC_BASE+0x0020)
#define CKGR_PLLR (PMC_BASE+0x002C)
#define PMC_MCKR (PMC_BASE+0x0030)
#define PMC_SR   (PMC_BASE+0x0068)

#define PIO_BASE 0xFFFFF400
#define PIO_PER  (PIO_BASE+0x0000)
#define PIO_PDR  (PIO_BASE+0x0004)
#define PIO_OER  (PIO_BASE+0x0010)
#define PIO_ODR  (PIO_BASE+0x0014)
#define PIO_SODR (PIO_BASE+0x0030)
#define PIO_CODR (PIO_BASE+0x0034)
#define PIO_PDSR (PIO_BASE+0x003C)
#define PIO_MDDR (PIO_BASE+0x0054)
#define PIO_PUDR (PIO_BASE+0x0060)
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

#define UART1_BASE 0xFFFC4000

#define US1_CR   (UART1_BASE+0x0000)
#define US1_MR   (UART1_BASE+0x0004)
#define US1_IER  (UART1_BASE+0x0008)
#define US1_IDR  (UART1_BASE+0x000C)
#define US1_IMR  (UART1_BASE+0x0010)
#define US1_CSR  (UART1_BASE+0x0014)
#define US1_RHR  (UART1_BASE+0x0018)
#define US1_THR  (UART1_BASE+0x001C)
#define US1_BRGR (UART1_BASE+0x0020)
#define US1_RTOR (UART1_BASE+0x0024)
#define US1_TTGR (UART1_BASE+0x0028)
#define US1_FIDI (UART1_BASE+0x0174)
#define US1_NER  (UART1_BASE+0x0044)
#define US1_IF   (UART1_BASE+0x004C)


void clock_init ( void )
{
    unsigned int ra;

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


    //setup PLL based clock
    //http://www.atmel.com/dyn/resources/prod_documents/AT91SAM_pll.htm

    PUT32(CKGR_MOR,0xFF01);
    //wait for the startup to end
    while(1) if(GET32(PMC_SR)&(1<<0)) break;
    //divide by 18, mul by 107 = 109.568 between 80 and 160
    //divide that by 2 to get almost almost 55mhz
    PUT32(CKGR_PLLR,((107-1)<<16)|(0x3F<<8)|(18));
    while(1) if(GET32(PMC_SR)&(1<<2)) break; //waitfor LOCK
    while(1) if(GET32(PMC_SR)&(1<<3)) break; //waitfor MCKRDY
    PUT32(PMC_MCKR,0x07); //MAIN clock after a divide by 2
    while(1) if(GET32(PMC_SR)&(1<<3)) break; //waitfor MCKRDY
}

void uart_init ( void )
{
    PUT32(PMC_PCER,1<<PIOA_ID);


    //PUT32(PMC_PCER,1<<US1_ID);
    ////PA21 RXD1 PA22 TXD1
    //PUT32(PIO_PDR,(1<<21)|(1<<22));
    //PUT32(PIO_ASR,(1<<21)|(1<<22));

    //PUT32(US1_CR,(1<<2)|(1<<3)|(1<<5)|(1<<7)); //reset and disable rx and tx

    //PUT32(US1_MR,(3<<6)|(4<<9));
    //PUT32(US1_IDR,0xFFFFFFFF);
    ////starting with 18432000 Hz
    ////18432000*107/36 = 54784000
    ////54784000/(16*115200) = 29.7222, use 30
    //PUT32(US1_BRGR,30);
    //PUT32(US1_RTOR,0);
    //PUT32(US1_TTGR,0);
    //PUT32(US1_FIDI,0);
    //PUT32(US1_IF,0);
    //PUT32(US1_CR,(1<<6)|(1<<4)); //enable TX and RX



    PUT32(PMC_PCER,1<<US0_ID);
    //PA5 RXD0  PA6 TXD0
    PUT32(PIO_PDR,(1<<5)|(1<<6));
    PUT32(PIO_ASR,(1<<5)|(1<<6));

    PUT32(US0_CR,(1<<2)|(1<<3)|(1<<5)|(1<<7)); //reset and disable rx and tx

    PUT32(US0_MR,(3<<6)|(4<<9));
    PUT32(US0_IDR,0xFFFFFFFF);
    //starting with 18432000 Hz
    //18432000*107/36 = 54784000
    //54784000/(16*115200) = 29.7222, use 30
    PUT32(US0_BRGR,30);
    PUT32(US0_RTOR,0);
    PUT32(US0_TTGR,0);
    PUT32(US0_FIDI,0);
    PUT32(US0_IF,0);
    PUT32(US0_CR,(1<<6)|(1<<4)); //enable TX and RX





}

//-------------------------------------------------------------------------
void uart_putc ( unsigned int c )
{
    while(1) if(GET32(US0_CSR)&(1<<9)) break;
    PUT32(US0_THR,c);
}
//-------------------------------------------------------------------------
unsigned int uart_getc ( void  )
{
    while(1) if(GET32(US0_CSR)&(1<<0)) break;
    return(GET32(US0_RHR));
}
//-------------------------------------------------------------------------
void uart1_putc ( unsigned int c )
{
    while(1) if(GET32(US1_CSR)&(1<<9)) break;
    PUT32(US1_THR,c);
}
//-------------------------------------------------------------------------
unsigned int uart1_getc ( void  )
{
    while(1) if(GET32(US1_CSR)&(1<<0)) break;
    return(GET32(US1_RHR));
}

//-------------------------------------------------------------------
void hexstring ( unsigned int d )
{
    //unsigned int ra;
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
    uart_putc(0x0D);
    uart_putc(0x0A);
}
//-------------------------------------------------------------------
void hexstrings ( unsigned int d )
{
    //unsigned int ra;
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
//-------------------------------------------------------------------


//-------------------------------------------------------------------

#define PDI_DIR_OUT PUT32(PIO_OER,1<<25)
#define PDI_DIR_IN  PUT32(PIO_ODR,1<<25)
#define SET_PDI_DATA PUT32(PIO_SODR,1<<25)
#define CLR_PDI_DATA PUT32(PIO_CODR,1<<25)
#define SET_PDI_CLK  PUT32(PIO_SODR,1<<27)
#define CLR_PDI_CLK  PUT32(PIO_CODR,1<<27)
#define GET_DATA_IN ((GET32(PIO_PDSR)>>25)&1)
#define DELX 10
//-------------------------------------------------------------------
void write_data_bit ( unsigned int x )
{
    if(x&1) SET_PDI_DATA; else CLR_PDI_DATA;
}
//-------------------------------------------------------------------
void gpio_init ( void )
{
    PUT32(PMC_PCER,1<<PIOA_ID);
    PUT32(PIO_PER,(1<<25)|(1<<27)); //enable port
    PUT32(PIO_OER,(1<<25)|(1<<27)); //make output
    PUT32(PIO_MDDR,(1<<25)|(1<<27)); //push-pull
    PUT32(PIO_PUDR,(1<<25)|(1<<27)); //no pull up
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
//-------------------------------------------------------------------
void send_pdi_command ( unsigned int x )
{
    unsigned int ra;
    unsigned int rb;

    //109876543210
    //xxp76543210s

    ra=0xFFFFFFFF;
    ra<<=1;
    if(parity[x&0xFF]) ra|=1;
    ra<<=8;
    ra|=x;
    ra<<=1;

    PDI_DIR_OUT;
    for(rb=0;rb<12;rb++)
    {
        CLR_PDI_CLK;
        write_data_bit(ra);
        ASMDELAY(DELX);

        SET_PDI_CLK;
        ra>>=1;
        ASMDELAY(DELX);
    }
}
//-------------------------------------------------------------------
void send_pdi_break ( unsigned int len )
{
    unsigned int ra;

    PDI_DIR_OUT;
    CLR_PDI_DATA;
    for(ra=0;ra<len;ra++)
    {
        CLR_PDI_CLK;
        ASMDELAY(DELX);
        SET_PDI_CLK;
        ASMDELAY(DELX);
    }

}
//-------------------------------------------------------------------
void send_pdi_idle ( unsigned int len )
{
    unsigned int ra;

    PDI_DIR_OUT;
    SET_PDI_DATA;
    for(ra=0;ra<len;ra++)
    {
        CLR_PDI_CLK;
        ASMDELAY(DELX);
        SET_PDI_CLK;
        ASMDELAY(DELX);
    }

}
//-------------------------------------------------------------------
unsigned int get_one_byte ( void )
{
    unsigned int ra,rb,rc,rd;

    PDI_DIR_IN;
    while(1)
    {
        CLR_PDI_CLK;
        ASMDELAY(DELX);
        SET_PDI_CLK;
        ra=GET_DATA_IN;
        ASMDELAY(DELX);
        if(ra==0) break;
    }
    rc=0;
    for(rb=0;rb<11;rb++)
    {
        CLR_PDI_CLK;
        ASMDELAY(DELX);
        SET_PDI_CLK;
        ra=GET_DATA_IN;
        ASMDELAY(DELX);
        rc<<=1;
        rc|=ra;
    }
    if((rc&0x803)!=0x003)
    {
        //framing error
        hexstring(0xBADBAD00);
        doexit(1);
    }
    rc>>=2;
    rd=rc&1;
    rc>>=1;
    rb=0;
    for(ra=0;ra<8;ra++)
    {
        rb<<=1;
        rb|=rc&1;
        rd+=rc&1;
        rc>>=1;
    }
    if(rd&1)
    {
        //parity error
        hexstring(0xBADBAD01);
        doexit(1);
    }
    return(rb);
}
//-------------------------------------------------------------------
void pdi_send_four_le ( unsigned int data )
{
    send_pdi_command(data&0xFF);
    data>>=8;
    send_pdi_command(data&0xFF);
    data>>=8;
    send_pdi_command(data&0xFF);
    data>>=8;
    send_pdi_command(data&0xFF);
}
//-------------------------------------------------------------------
unsigned int pdi_get_four_le ( void )
{
    unsigned int ra;
    unsigned int rb;

    ra=get_one_byte();
    rb=get_one_byte();
    ra|=rb<<8;
    rb=get_one_byte();
    ra|=rb<<16;
    rb=get_one_byte();
    ra|=rb<<24;
    return(ra);
}
//-------------------------------------------------------------------
unsigned int pdi_ldcs ( unsigned int reg )
{
    unsigned int ra;

    send_pdi_command(0x80|reg);
    ra=get_one_byte();
    return(ra);
}
//-------------------------------------------------------------------
void pdi_stcs ( unsigned int reg, unsigned int data )
{
    send_pdi_command(0xC0|reg);
    send_pdi_command(data&0xFF);
}
//-------------------------------------------------------------------
void pdi_put8 (  unsigned int address, unsigned int data )
{
    send_pdi_command(0x4C);
    pdi_send_four_le(address);
    send_pdi_command(data&0xFF);
}
//-------------------------------------------------------------------
unsigned int pdi_get8 (  unsigned int address )
{
    unsigned int ra;

    send_pdi_command(0x0C);
    pdi_send_four_le(address);
    ra=get_one_byte();
    return(ra);
}
//-------------------------------------------------------------------
void pdi_put16 (  unsigned int address, unsigned int data )
{
    send_pdi_command(0x4D);
    pdi_send_four_le(address);
    send_pdi_command((data>>0)&0xFF);
    send_pdi_command((data>>8)&0xFF);
}
//-------------------------------------------------------------------
void pdi_put32 (  unsigned int address, unsigned int data )
{
    send_pdi_command(0x4F);
    pdi_send_four_le(address);
    pdi_send_four_le(data);
}
//-------------------------------------------------------------------
unsigned int pdi_get32 (  unsigned int address )
{
    unsigned int ra;

    send_pdi_command(0x0F);
    pdi_send_four_le(address);
    ra=pdi_get_four_le();

    return(ra);
}
//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------
unsigned int pdi_init ( void )
{
    unsigned int ra;

    gpio_init();

    PDI_DIR_OUT;
    SET_PDI_CLK;
    SET_PDI_DATA;
    ASMDELAY(5000);
    send_pdi_idle(200);
    send_pdi_break(24);
    send_pdi_idle(24);

    //put avr core in reset
    send_pdi_command(0xC1);
    send_pdi_command(0x59);
    //check status, verify in reset
    ra=pdi_ldcs(0x01);
if(0)
{
    hexstring(ra);
    doexit(1);
}
    if(ra!=1)
    {
        hexstring(ra);
        hexstring(0xBADBAD00);
        return(1);
    }
    //change guard time to 8+ clocks, makes this much faster
    pdi_stcs(0x02,0x04);

    //read id and rev
    ra=pdi_get32(0x01000090);
if(0)
{
    hexstring(ra);
    doexit(1);
}
    if((ra&0x00FFFFFF)!=0x004C971E)
    {
        hexstring(ra);
        hexstring(0xBADBAD01);
        return(1);
    }
    return(0);
}
//-------------------------------------------------------------------
unsigned int unlock_nvm ( void )
{
    unsigned int ra;
    unsigned int rb;

    //unlock nvm
    ra=pdi_ldcs(0x00);
if(0)
{
    hexstring(ra);
    doexit(1);
}
    if((ra&2)==0)
    {
        send_pdi_command(0xE0);
        send_pdi_command(0xFF);
        send_pdi_command(0x88);
        send_pdi_command(0xD8);
        send_pdi_command(0xCD);
        send_pdi_command(0x45);
        send_pdi_command(0xAB);
        send_pdi_command(0x89);
        send_pdi_command(0x12);
        for(rb=0;rb<100;rb++)
        {
            ra=pdi_ldcs(0x00);
            if((ra&2)==2) break;
        }
        if(rb>=100)
        {
            hexstring(0xBADBAD02);
            return(1);
        }
    }
    //NVM Unlocked
    return(0);
}
//-------------------------------------------------------------------
unsigned int nvm_chip_erase ( void )
{
    unsigned int ra;
    unsigned int rb;

    pdi_put8(0x010001CA,0x40); //chip erase
    pdi_put8(0x010001CB,0x01); //cmdex
    for(ra=0;;ra++)
    {
        rb=pdi_ldcs(0x00);
        if(rb&2) break;
    }
if(0)
{
    hexstring(ra);
    hexstring(rb);
    doexit(1);
}
    return(0);
}
//-------------------------------------------------------------------
#define BOOT_PAGE_SIZE 0x100
#define BOOT_BASE_ADD  0x00820000
//-------------------------------------------------------------------
unsigned int load_bootloader_flash ( unsigned short *bin, unsigned int len )
{
    unsigned int ra;
    unsigned int rb;
    unsigned int add;
    unsigned int base;

    add=0;
    while(add<len)
    {
        base=add;
        //load flash page buffer
        pdi_put8(0x010001CA,0x23);
        for(ra=0;ra<BOOT_PAGE_SIZE;ra+=2)
        {
            if(add<len)
            {
                pdi_put8(BOOT_BASE_ADD+add+0,(bin[add>>1]>>0)&0xFF);
                pdi_put8(BOOT_BASE_ADD+add+1,(bin[add>>1]>>8)&0xFF);
            }
            else
            {
                pdi_put8(BOOT_BASE_ADD+add+0,0xFF);
                pdi_put8(BOOT_BASE_ADD+add+1,0xFF);
            }
            add+=2;
        }
        ra=pdi_get8(0x010001CF);
        rb=pdi_get8(0x010001CF);
        if(0)
        {
            hexstring(ra);
            hexstring(rb);
            doexit(1);
        }

        pdi_put8(0x010001CA,0x2C);
        pdi_put8(BOOT_BASE_ADD+base,0x00);
        for(ra=0;;ra++)
        {
            rb=pdi_get8(0x010001CF);
            if(rb==0) break;
        }
        if(0)
        {
            hexstring(ra);
            hexstring(rb);
            doexit(1);
        }

    }

if(0)
{
    unsigned char some_data[0x200];

    pdi_put8(0x010001CA,0x43);
    for(ra=0;ra<0x200;ra++)
    {
        some_data[ra]=pdi_get8(0x00820000+ra);
    }
    for(ra=0;ra<0x200;ra++)
    {
        hexstrings(ra); hexstring(some_data[ra]);
    }
    doexit(1);
}


    return(0);
}
//-------------------------------------------------------------------
unsigned int pdi_close ( void )
{
    send_pdi_command(0xC1);
    send_pdi_command(0x00);
    return(0);
}
//-------------------------------------------------------------------
void notmain ( void )
{
    clock_init();
    uart_init();
    hexstring(0x12345678);

    //Once you start talking on the pdi bus, you have to keep the clock
    //running, bit banging in this manner that means you cannot stop
    //to print things out, no printing of messages on the uart unless
    //you plan to stop communication with the xmega, and/or unless
    //you plan to start over initializing communication with the part.
    if(pdi_init()) doexit(1);
    if(unlock_nvm()) doexit(1);
    if(nvm_chip_erase()) doexit(1);
    if(load_bootloader_flash((unsigned short *)rom,ROM_LENGTH<<1)) doexit(1);
    if(pdi_close()) doexit(1);
    // can print now and do whatever, want to let the pdi clock line stop
    // which will let the part run normally.

    send_pdi_idle(200);
    send_pdi_break(24);
    send_pdi_idle(24);

    hexstring(0xAABBCCDD);
    hexstring(0x12345678);
}
//-------------------------------------------------------------------
//-------------------------------------------------------------------
