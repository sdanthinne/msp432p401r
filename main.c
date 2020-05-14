#include "spi.h"
#include "DCO.h"
#include "DAC.h"
#include "delay.h"
#include "fgen.h"

/**
 * main.c
 */

void main(void)
{

    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD; // stop watchdog timer
    P6->DIR|=BIT0; //FOR DIAGNOSTICS/testing

    main_fg();
//    while(1)
//    {
//        P6->OUT^=BIT0;//WARNING: ADDING THIS LINE SIGNIFICANTLY INCREASES PERIOD
//
//    }
}
