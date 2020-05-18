#include "msp.h"
#include "DCO.h"
/*
 * DCO.c
 *
 *  Created on: Apr 9, 2020
 *      Author: sdanthinne
 */
void setDCO(uint32_t freq)
{
    CS->KEY=CS_KEY_VAL; // unlocks cs registers
    CS->CTL0=freq; // set the DCO frequency to freq
    CS->CTL1 |= CS_CTL1_SELM__DCOCLK; // sets the MCLK to use DCO
    CS->KEY=0; //re-lock CS registers
}

/**
 * returns the value of the DCO
 * in MHz
 */
float get_DCO_speed()
{
    uint32_t dcoVal;
    CS->KEY=CS_KEY_VAL;

    switch(CS->CTL0&CS_CTL0_DCORSEL_MASK)
    {
    case FREQ_1_5_MHz:
        dcoVal = 1.5;
        break;

    case FREQ_3_MHz:
        dcoVal = 3;
        break;

    case FREQ_6_MHz:
        dcoVal = 6;
        break;

    case FREQ_12_MHz:
        dcoVal = 12;
        break;

    case FREQ_24_MHz:
        dcoVal = 24;
        break;

    case FREQ_48_MHz:
        dcoVal = 48;
        break;

    default:
        dcoVal = 0;
        break;
    }
    return dcoVal;
}
/**
 * sets the smclk to source from the DCO
 */
void set_SM_DCO()
{
    setDCO(FREQ_24_MHz);
    CS->KEY=CS_KEY_VAL; // unlocks cs registers
    CS->CTL1 |= CS_CTL1_SELS__DCOCLK; // sets the SMCLK to use DCO
    CS->KEY=0; //re-lock CS registers

}
