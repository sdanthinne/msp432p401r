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
   // set_timer_square();
    write_TRIANGLE();
    while(1);
//    while(1)
//    {
//        write_DAC(wave[val]);
//        val++;
//        val = (val>511)? 0:val;
//        delay_us(1000);
//    }

}
