#include "spi.h"
#include "DCO.h"



/**
 * main.c
 */
void main(void)
{
    uint16_t data=1;
    uint16_t adder=2;
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD; // stop watchdog timer
    setDCO(FREQ_1_5_MHz);
    set_SM_DCO();

    __disable_irq(); // disable global interrupts
    setup_SPI_b0();
    __enable_irq(); //enable globular interrupts

//    data = 0x3000;
//    write_byte_b0(data>>8);
//    write_byte_b0_cont(data);
//	while(1)
//	{
//	    data = 0x3000 | (adder);
//	    write_byte_b0(data>>8);
//	    write_byte_b0_cont(data);
//	    if(adder>=0x1000)
//	    {
//	        adder=0;
//	    }
//	    adder*=2;
//	}
    write_DAC(512);
//    while(1)
//    {
//        write_DAC(data);
//        data*=adder;
//    }
}
