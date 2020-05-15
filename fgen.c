#include "DCO.h"
#include "fgen.h"
#include "timer.h"
#include "DAC.h"
#include "keypad.h"
/*
 * fgen.c
 *
 *  Created on: May 11, 2020
 *      Author: sdanthinne aknapen crapp
 * Contains all of the logic for CPE 316 P2.
 * assumes that we are running DCO speed at 24MHz.
 * outputs:
 *  square wave
 *  sine wave
 *  sawtooth wave
 *  trianglewave
 *  Superpostion of any waves
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
    TIMER_A0->CTL |=
            TIMER_A_CTL_SSEL__SMCLK | // sets timer's source as SMCLK
            TIMER_A_CTL_MC__CONTINUOUS; // sets timer to CONTINUOUS mode

    TIMER_A0->CCTL[0] = TIMER_A_CCTLN_CCIE; // TACCR0 interrupt enable
    TIMER_A0->CCR[0] = time*DCO_SPEED;
    NVIC->ISER[0] = 1 << (TA0_0_IRQn&31);//enable interrupts for below routine
    // assigns interrupt priority 1 (second highest) to TimerA0 in NVIC
    NVIC->IP[2] |= BIT0;
    __enable_irq();
}

/**
 * sets up the function generator for use
 */
void setup_fg(void)
{
    setDCO(FREQ_24_MHz);//we want the highest to get the best output
    set_timer_fg(SAMPLE_TIME);//setup the timer to interrupt every SAMPLE_TIME
    setup_DAC();
    setup_keypad();
    is_ready = 0;//initialize the flag to zero
}

/**
 * Returns the value of all currently active waves superimposed onto eachother
 */
uint16_t get_value(uint32_t count,uint8_t wave_type)
{
    // value to be returned
    uint16_t out_value = 0;

    // Does math to determine maximum number of updates
    //in a period given frequency of interrupts and of wave
    uint16_t max_count = (INTERRUPT_FREQUENCY / frequency);

    // Normalizes the count to be within 0 and the maximum value
    uint16_t normalized_count = count % max_count;

    // Computes the index in the LUT given the count
    uint16_t array_val = (512 * normalized_count) / (max_count);

    out_value=// If Sawtooth is active add value to total
              waves_3v[SAWTOOTH_WAVE&wave_type][array_val] +
              // If Sinwave is active add value to total
              waves_3v[SINE_WAVE&wave_type][array_val] +
              // If Triangle is active add value to total
              waves_3v[TRIANGLE_WAVE&wave_type][array_val] +
              // If Squarewave is active add value to total
              waves_3v[SQUARE_WAVE&wave_type][SQUARE_INDEX];

    // Divide by the number of active waves to scaled superimposed wave
    return out_value/wave_count;
}

/**
 * our function generator's main.
 */
void main_fg(void)
{
    uint16_t value = 0;
    uint32_t count = 0;

    setup_fg();//run setup
    wave_type = SQUARE_WAVE;//select the default waveform to output
    wave_count = 1;
    duty_cycle = 5;
    frequency = 100;//set the frequency

    while(1)   // Function loop
    {
        value = get_value(count++,wave_type);//get the value
        while(!is_ready);//wait until the interrupt says to write a value
        write_DAC(value);//write the value
        is_ready = 0;//set our homemade flag to 0
    }
}

/**
 * for every time period SAMPLE_TIME, then
 */
void TA0_0_IRQHandler(void)
{
    TIMER_A0->CCTL[0] &= ~TIMER_A_CCTLN_CCIFG;//clear flag
    is_ready = 1;       // Sets our check flag to output
    TIMER_A0->CCR[0]+=TIMER_VALUE;//go for next sample time
}
