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
 * sets the smclk to source from the DCO
 */
void set_SM_DCO()
{
    CS->KEY=CS_KEY_VAL; // unlocks cs registers
    CS->CTL1 |= CS_CTL1_SELS__DCOCLK; // sets the SMCLK to use DCO
    CS->KEY=0; //re-lock CS registers

}
