#include "spi.h"
#include "DCO.h"
#include "DAC.h"
#include "delay.h"


/**
 * main.c
 */

void main(void)
{

    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD; // stop watchdog timer



    setup_DAC();

}
