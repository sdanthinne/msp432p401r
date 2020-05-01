#include "timer.h"


void setup_pin()
{
    // Set up output signal for ISR
    P2->DIR |= BIT7; // configure P2.7 as output
    P2->OUT |= BIT7;

    P2->SEL0 &= ~BIT7; // configure P2.7 as GPIO
    P2->SEL1 &= ~BIT7;

    // Set up signal to time ISR
    P2->DIR |= BIT5; // configure P2.5 as output
    P2->OUT |= BIT5;

    P2->SEL0 &= ~BIT5; // configure P2.5 as GPIO
    P2->SEL1 &= ~BIT5;

//    // Set up signal to measure MCLK
//    P4->DIR |= BIT3; // configure P4.3 as output
//    P4->OUT |= BIT3;
//
//    P4->SEL0 |= BIT3; // configure P4.3 for primary function (MCLK)
//    P4->SEL1 &= ~BIT3;

}

/**
 * main.c
 */
void main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD; // stop watchdog timer
    __disable_irq();
    setup_pin();
      //NVIC_EnableIRQ(TA0_0_IRQn);
      //NVIC_EnableIRQ(TA0_N_IRQn);
    NVIC->ISER[0] = 1 << (TA0_0_IRQn&31); // enable TimerA0's interrupts

    NVIC->ISER[0] = 1 << (TA0_N_IRQn&31); // enable TimerA1's interrupts

	set_timer();
    __enable_irq(); // enable global interrupts

	while(1);
}

// Timer A0's CCR0 interrupt service routine
void TA0_0_IRQHandler(void)
{
    //P2->OUT |= BIT5; // set P2.5 high at beginning of ISR

    TIMER_A0->CCTL[0] &= ~TIMER_A_CCTLN_CCIFG; // clears capture/compare interrupt flag
    //TIMER_A0->CTL &= ~BIT0;
    P2->OUT ^= BIT7; // toggles P2.7 when TA0_CCR0 reached
    TIMER_A0->CCR[0] +=1500;


    //P2->OUT &=   ~BIT5; // clear P2.5 at the end of the ISR
    // Modified CCR0 ISR for Part B of A5

}

// Timer A0's CCR1 interrupt service routine
void TA0_N_IRQHandler(void)
{
    if(TIMER_A0->IV==2)
    {
        //TIMER_A0->CCTL[1] &= ~TIMER_A_CCTLN_CCIFG; // clears capture/compare interrupt flag
        P2->OUT ^= BIT5; // sets P2.5 low when TA0_CCR1 reached
        TIMER_A0->CCR[1] +=1500/2;
    }
}
