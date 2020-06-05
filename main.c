/*
 * main.c
 *
 *  Created on: May 18, 2020
 *      Authors: aknapen, crapp, sdanthinne
 */

#include "msp.h"
#include "uart.h"
#include "DAC.h"


void main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer

    //setup_DAC(); // configure use of SPI with the DAC
    setup_uart(); // Configure use of UART
    setup_ADC();

    P1->DIR |= BIT0;
    P1->SEL0 &= ~BIT0;
    P1->SEL1 &= ~BIT0;
    P1->OUT &= ~BIT0;

    while (1)
    {
        while(!is_ready); // wait until the program read to write to DAC

    }
}
