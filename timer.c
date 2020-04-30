#include "timer.h"
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

void set_timer()
{
    set_DCO(FREQ_24_MHZ);

    TIMER_A0->CTL |= TIMER_A_CTL_SSEL__SMCLK | // sets timer's source as SMCLK
                     TIMER_A_CTL_MC__UP; // sets timer to UP mode

    TIMER_A0->CCTL[0] = TIMER_A_CCTLN_CCIE; // TACCR0 interrupt enable
    //TIMER_A0->CCTL[1] = TIMER_A_CCTLN_CCIE; // TACCR1 interrupt enable

    TIMER_A0->CCR[0] = 480; // Takes 40 us to accumulate
   // TIMER_A0->CCR[1] = 240; // Takes 10 us to accumulate

//    __enable_irq(); // enable global interrupts
//    NVIC->ISER[0] = 1 << TA0_0_IRQn; // enable TimerA0's interrupts
//    NVIC->ISER[0] = 1 << TA0_N_IRQn; // enable TimerA1's interrupts

}
