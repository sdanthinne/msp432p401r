#include "delay.h"
/*
 * delay.c
 *
 *  Created on: Apr 10, 2020
 *      Author: sdanthinne,aknapen,crapp
 *      contains functions used to delay
 */

/**
 * delays in microseconds up to 50 ms
 * assumes that the MCLK is DCO driven
 * DCO configuration taken from CSCTL0
 * rounds up to the next highest delay if there cannot be an exact one.
 */

void delay_us(uint32_t delay)
{
    uint32_t dco_freq, freq_factor, num_cycles=0, i;

    dco_freq = (CS->CTL0 & (BIT(16)|BIT(17)|BIT(18)));
    freq_factor = 1 << (dco_freq >>16);
    num_cycles = (delay * freq_factor)-STATIC_OVERHEAD;

    if(num_cycles<1000)
    {
        num_cycles /=SMALL_CYC_DIV;
    }else
    {
        num_cycles /=BIG_CYC_DIV;
    }
    for(i=0 ; i<num_cycles; i++)
    {
        __NOP();
    }
}


