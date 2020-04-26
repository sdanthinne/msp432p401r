#include "delay.h"
/*
 * delay.c
 *
 *  Created on: Apr 10, 2020
 *      Author: sdanthinne
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
    num_cycles = (delay * freq_factor)-40;

    if(num_cycles<1000)
    {
        num_cycles /=7;
    }else
    {
        num_cycles /=8;
    }
    for(i=0 ; i<num_cycles; i++)
    {
        __NOP();
    }
//    uint32_t i,times=0,dcoF = (CS->CTL0 & (BIT(16)|BIT(17)|BIT(18)))>>16;
//    switch(dcoF)
//    {
//    case 0:
//        times = (3*(delay-OVERHEAD1)/2)/DIV_AMT;
//        break;
//    case 1:
//        times = ((delay-OVERHEAD3)*3)/DIV_AMT;
//        break;
//    case 2:
//        times = ((delay-OVERHEAD6)*6)/DIV_AMT;
//        break;
//    case 3:
//        times = ((delay-OVERHEAD12)*12)/DIV_AMT;
//        break;
//    case 4:
//        times = ((delay-OVERHEAD24)*24)/DIV_AMT;
//        break;
//    case 5:
//        times= ((delay-OVERHEAD48)*48)/DIV_AMT;
//        break;
//    }
//    for(i=0; i<times;i++)//each iteration of this loop takes 5 cycles
//    {
//        __NOP();
//    }

}


