#include "ADC.h"
#include "delay.h"
/*
 * ADC.c
 *
 *  Created on: Jun 5, 2020
 *      Author: sdanthinne, aknapen, crapp
 */

void setup_ADC()
{
    ADC14->CTL0 &= ~ADC14_CTL0_ENC; // disable conversion during setup

    ADC14->CTL0 |= ADC14_CTL0_ON | // turn on ADC
                   ADC14_CTL0_SHP | // SAMPCON signal is sourced from the sampling time
                   ADC14_CTL0_SHT0_0 | // 4 cycle sample-hold time
                   ADC14_CTL0_SSEL__SMCLK | // use SMCLK as ADC clk src
                   ADC14_CTL0_PDIV__4; // predivide ADC clk by 4

    ADC14->CTL0 &= ~(ADC14_CTL0_CONSEQ_MASK | // single-channel, single-conversion mode
                     ADC14_CTL0_DIV_MASK | // divide ADC clk by 1
                     ADC14_CTL0_ISSH | // sample input signal not inverted
                     ADC14_CTL0_SHS_MASK); // use ADC14SC bit as sample-and-hold source select

    ADC14->CTL1 |= (ADC14_CTL1_RES__14BIT); // use 14-bit resolution

    ADC14->CTL1 &= ~(ADC14_CTL1_PWRMD_MASK | // regular power mode
                     ADC14_CTL1_REFBURST | // continuous reference buffer burst
                     ADC14_CTL1_DF | // read data as unsigned binary
                     ADC14_CTL1_CSTARTADD_MASK); // use ADC14MEM0 for conversion memory register

    /*
     *  disable window comparator
     *  use single-ended mode
     *  use AVCC/AVSS as reference voltages
     *  A0 selected as input channel
     */
    ADC14->MCTL[0] = 0;

    ADC14->CTL0 |= ADC14_CTL0_ENC; // re-enable conversion after setup
}

/**
 * interrupt handler for ADC conversions
 */
void ADC14_IRQHandler(void)
{

}

uint16_t read_ADC()
{
    ADC14->CTL0 |= ADC14_CTL0_SC; // start ADC conversion

    while(!(ADC14->CTL0 & ADC14_CTL0_BUSY) != 0); // wait until value has been converted

    EUSCI_A0->TXBUF = ADC14->MEM[0];
    return ADC14->MEM[0] & ADC14_MEMN_CONVRES_MASK; // extract conversion value from memory
}
