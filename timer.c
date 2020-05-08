#include "timer.h"
#include "DAC.h"
/*
 * timer.c
 *
 *  Created on: Apr 20, 2020
 *      Author: Alexander
 */


void set_DCO(uint32_t selected_freq)
{
    CS->KEY = CS_KEY_VAL; // Unlocks the CS

    CS->CTL0 = selected_freq; // Sets DCO to one of the nominal frequencies

    CS->CTL1 |= CS_CTL1_SELS__DCOCLK; // Sets the SMCLK source to the DCO
    CS->CTL1 |= CS_CTL1_DIVS__1; // Sets the SMCLK divider to be 1

    CS->KEY = 0; // Locks the CS again
}

// Timer A0's CCR0 interrupt service routine
void TA0_0_IRQHandler(void)
{
    TIMER_A0->CCTL[0] &= ~TIMER_A_CCTLN_CCIFG; // clears capture/compare interrupt flag
    write_DAC(HIGH_VOLTAGE); // output high 2Vpp with 1V DC offset
    TIMER_A0->CCR[0] += TWENTY_MS;
}

// Timer A0's CCR1 and CCR2 interrupt service routine
void TA0_N_IRQHandler(void)
{
    static uint8_t sample_count = 0;
    static uint16_t dac_in = LOW_VOLTAGE; // starting DAC input value
    static uint8_t counting_up = 1;

    if(TIMER_A0->IV == 4)       // If interrupt source is Timer A0 CCR2
    {
        TIMER_A0->CCR[2] += SAMPLE_INTERVAL; // Increment CCR value


        if (sample_count >= MAX_NUM_SAMPLES_1)
        {
            counting_up = (counting_up + 1) % 2; // Reverse count direction
        }

        sample_count = (sample_count+1) % MAX_NUM_SAMPLES;

        counting_up ? (dac_in+=INCREMENT) : (dac_in-=INCREMENT);
        write_DAC(dac_in); // output high 2Vpp with 1V DC offset
    }

    else if (TIMER_A0->IV == 2) // If interrupt source is Timer A0 CCR1
    {
        write_DAC(LOW_VOLTAGE); // output low 2Vpp with 1V DC offset
        TIMER_A0->CCR[1] += TWENTY_MS;
    }

}
