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

    write_DAC(512);


    while(1);
}
