/*
 * ADC.c
 *
 *  Created on: Jun 5, 2020
 *      Author: sdanthinne
 */

void setup_ADC()
{

    BITBAND_PERI(ADC14->CTL0, ADC14_CTL0_ENC_OFS) = 0;//disable ADC

}

/**
 * interrupt handler for ADC conversions
 */
void ADC14_IRQHandler(void)
{

}
