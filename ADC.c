#include "ADC.h"
/*
 * ADC.c
 *
 *  Created on: Jun 5, 2020
 *      Author: sdanthinne, aknapen, crapp
 */

void setup_ADC()
{
//    ADC14->CTL0 &= ~ADC14_CTL0_ENC; // disable conversion during setup
//
//    ADC14->CTL0 |= ADC14_CTL0_ON | // turn on ADC
//                   ADC14_CTL0_SSEL__SMCLK | // use SMCLK as ADC clk src
//                   ADC14_CTL0_PDIV__4; // predivide ADC clk by 4
//
//    ADC14->CTL0 &= ~(ADC14_CTL0_CONSEQ_MASK | // single-channel, single-conversion mode
//                     ADC14_CTL0_DIV_MASK | // divide ADC clk by 1
//                     ADC14_CTL0_ISSH | // sample input signal not inverted
//                     ADC14_CTL0_SHS_MASK); // use ADC14SC bit as sample-and-hold source select
//
//    ADC14->CTL1 |= (ADC14_CTL1_RES__14BIT); // use 14-bit resolution
//
//    ADC14->CTL1 &= ~(ADC14_CTL1_PWRMD_MASK | // regular power mode
//                     ADC14_CTL1_REFBURST | // continuous reference buffer burst
//                     ADC14_CTL1_DF | // read data as unsigned binary
//                     ADC14_CTL1_CSTARTADD_MASK); // use ADC14MEM0 for conversion memory register
//
//    /*
//     *  disable window comparator
//     *  use single-ended mode
//     *  use AVCC/AVSS as reference voltages
//     *  A0 selected as input channel
//     */
//    ADC14->MCTL[0] = 0;
//
//    ADC14->CTL0 |= ADC14_CTL0_ENC; // re-enable conversion after setup

    ADC14->CTL0 = ADC14_CTL0_SHT0_1 | ADC14_CTL0_SHP | ADC14_CTL0_ON | ADC14_CTL0_SSEL_3;
    ADC14->CTL1 = ADC14_CTL1_RES_3; //ADC resolution set to 14-bit
    ADC14->MCTL[0] |= (ADC14_MCTLN_INCH_11 | ADC14_MCTLN_VRSEL_0 ); // channel selection and VeRef+, VeRef-
//    ADC14->IER0 |= ADC14_IER0_IE0; // ADC ISR setup
}

/**
 * interrupt handler for ADC conversions
 */
void ADC14_IRQHandler(void)
{

}

//uint16_t read_ADC()
//{
//    ADC14->CTL0 |= ADC14_CTL0_SC; // start ADC conversion
//
//    while(!(ADC14->IFGR0 & ADC14_IFGR0_IFG0)); // wait until value has been converted
//
//    P1->OUT ^= BIT0;
//    return ADC14->MEM[0] & ADC14_MEMN_CONVRES_MASK; // extract conversion value from memory
//}

int read_ADC(void){
    // Start sampling/conversion
    ADC14->CTL0 |= 0x00000001; // start conversion
    // Wait for conversion to complete and grab value
    // Conversion is complete when ADC0 flag is set
    while (!ADC14->IFGR0){
    ;
    }
    // returning a full int instead of a uint16_t for simplicity
    return ADC14->MEM[0];
}
