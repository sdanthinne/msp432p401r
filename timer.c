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

void set_timer()
{
    set_DCO(FREQ_1_5_MHZ);

    TIMER_A0->CTL |= TIMER_A_CTL_SSEL__SMCLK | // sets timer's source as SMCLK
            TIMER_A_CTL_MC__CONTINUOUS; // sets timer to UP mode

    TIMER_A0->CCTL[0] = TIMER_A_CCTLN_CCIE; // TACCR0 interrupt enable
    TIMER_A0->CCTL[1] = TIMER_A_CCTLN_CCIE; // TACCR1 interrupt enable

    TIMER_A0->CCR[0] = 3750; // Takes 20 ms to accumulate
    TIMER_A0->CCR[1] = 1875; // Takes 10 ms to accumulate

    __enable_irq(); // enable global interrupts
    NVIC->ISER[0] |= 1 << TA0_0_IRQn; // enable TimerA0's interrupts
    NVIC->ISER[0] |= 1 << TA0_N_IRQn; // enable TimerA1's interrupts

}

// Timer A0's CCR0 interrupt service routine
void TA0_0_IRQHandler(void)
{

    TIMER_A0->CCTL[0] &= ~TIMER_A_CCTLN_CCIFG; // clears capture/compare interrupt flag
    write_DAC(930); // output high 2Vpp with 1V DC offset
    TIMER_A0->CCR[0] += 3750;
}

// Timer A0's CCR1 interrupt service routine
void TA0_N_IRQHandler(void)
{
    TIMER_A0->CCTL[1] &= ~TIMER_A_CCTLN_CCIFG; // clears capture/compare interrupt flag
    write_DAC(310); // output low 2Vpp with 1V DC offset
    TIMER_A0->CCR[1] += 3750;
}
