/*
 * main.c
 *
 *  Created on: May 18, 2020
 *      Authors: aknapen, crapp, sdanthinne
 */

#include "msp.h"
#include "uart.h"
#include "bluetooth.h"
#include "DAC.h"
#include "arduino.h"
#include "ADC.h"


void main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer
    uint16_t adc_val;

    P5->SEL0 &= ~(BIT5); // Configure P5.5 to primary module function
    P5->SEL1 |= BIT5; // P5.5 becomes A0
    setup_uart();
    setup_ADC();



    while(1)
    {
        adc_val = read_ADC();
    }





}
