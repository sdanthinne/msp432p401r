/*
 * ADC.c
 *
 *  Created on: Jun 5, 2020
 *      Author: sdanthinne, aknapen, crapp
 */

void setup_ADC()
{

//    BITBAND_PERI(ADC14->CTL0, ADC14_CTL0_ENC_OFS) = 0;//disable ADC
    ADC14->CTL0 |= (ADC14_CTL0_PDIV_1 |  //Pre-divide clock by 4
            ADC14_CTL0_DIV_0 | // Divide clock by 1
            ADC14_CTL0_SSEL__SMCLK | // Set clock source to SM Clock
            ADC14_CTL0_CONSEQ_0 | // Single channel single conversion mode
            ADC14_CTL0_ON); // Turn the ADC on

    ADC14->CTL1 |= ADC14_CTL1_RES__10BIT; // 10 bit ADC resolution;

    ADC14->MCTL[0] |= ADC14_MCTLN_INCH_0; // Select P5.5 / A0 as ADC input

    ADC14->CTL0 |= ADC14_CTL0_ENC; // Enable conversion

}

/**
 * interrupt handler for ADC conversions
 */
void ADC14_IRQHandler(void)
{

}

void read_ADC()
{
    ADC14->CTL0 |= ADC14_CTL0_SC; // Start Conversion

    while(!(ADC14->IFGR0 & ADC14_IFGR0_IFG0)); // Wait for conversion to finish

    return ADC14->MEM[0];    // Return result of conversion
}
