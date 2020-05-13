#include "DCO.h"
#include "fgen.h"
#include "timer.h"
#include "DAC.h"
/*
 * fgen.c
 *
 *  Created on: May 11, 2020
 *      Author: sdanthinne
 * Contains all of the logic for CPE 316 P2.
 * assumes that we are running DCO speed at 24MHz.
 * outputs:
 *  square wave
 *  sine wave
 *  sawtooth wave
 * Frequencies:
 *  100 Hz
 *  200 Hz
 *  300 Hz
 *  400 Hz
 *  500 Hz
 * Amplitude:
 *  Vpp = 3V
 *  DC_Offset = 1.5v
 *
 * Translated in terms of the DCO:
 * Amplitude:
 *  between 0 and 931 (0-3)V
 *
 */



/**
 * sets the time interval for the CCR1 in order to get the
 * time is in us.
 *
 */
void set_timer_fg(uint16_t time)
{
    //do some calculation in here to determine what value to put into the CCR1
    time = time/(1/(DCO_SPEED));//calculates the CCRO time that is required

    TIMER_A0->CTL |=
            TIMER_A_CTL_SSEL__SMCLK | // sets timer's source as SMCLK
            TIMER_A_CTL_MC__CONTINUOUS; // sets timer to CONTINUOUS mode

    TIMER_A0->CCTL[0] = TIMER_A_CCTLN_CCIE; // TACCR0 interrupt enable
    TIMER_A0->CCR[0] = time;
    NVIC->ISER[0] = 1 << (TA0_0_IRQn&31);//enable interrupts for below routine
    __enable_irq();
}

/**
 * TODO: enable interrupts - set the CCR to the correct time
 */
void setup_fg(void)
{
    setDCO(FREQ_24_MHz);
    set_timer_fg(SAMPLE_TIME);//setup the timer to interrupt every SAMPLE_TIME
    setup_DAC();

}

/**
 * our function generator's main.
 */
void main_fg(void)
{
    setup_fg();

}

/**
 * for every time period SAMPLE_TIME, then
 */
void TA0_0_IRQHandler(void)
{
    TIMER_A0->CCTL[0] &= ~TIMER_A_CCTLN_CCIFG;//clear flag
    TIMER_A0->CCR[0]+=SAMPLE_TIME;//go for next sample time

    return;
}
