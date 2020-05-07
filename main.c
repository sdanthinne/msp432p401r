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
    setup_SPI_b0();

//    while(1)
//	{
//	    write_DAC(data);
//	    if(adder>=0x1000)
//	    {
//	        adder=0;
//	    }
//	    adder*=2;
//	}
    write_DAC(1000);

    while(1)
    {
        write_DAC(1000);
    }
}
