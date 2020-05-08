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
    write_DAC(930); // output high 2Vpp with 1V DC offset
    TIMER_A0->CCR[0] += 3750;
}

// Timer A0's CCR1 and CCR2 interrupt service routine
void TA0_N_IRQHandler(void)
{
    static uint16_t triangle_val = 0;
    static uint8_t counting_up = 0;

    if(TIMER_A0->IV == 4)       // If interrupt source is Timer A0 CCR2
      {
          uint16_t voltage_val;
          TIMER_A0->CCR[2] += 61;
          if(triangle_val == 620)
          {
              counting_up = 0;
          }
          if(triangle_val == 0)
          {
              counting_up = 1;
          }
          counting_up ? (triangle_val+=20) : (triangle_val-=20);

          voltage_val = 310 + triangle_val;
          TIMER_A0->CCTL[2] &= ~TIMER_A_CCTLN_CCIFG; // clears capture/compare interrupt flag
          write_DAC(voltage_val); // output high 2Vpp with 1V DC offset

      }

    else if (TIMER_A0->IV == 2) // CCR1 source of interrupt
    {
        TIMER_A0->CCTL[1] &= ~TIMER_A_CCTLN_CCIFG; // clears capture/compare interrupt flag
        write_DAC(310); // output low 2Vpp with 1V DC offset
        TIMER_A0->CCR[1] += 3750;
    }



}
