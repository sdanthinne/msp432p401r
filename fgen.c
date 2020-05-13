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

uint8_t wave_type;//the type of the wave - refer to macros
uint8_t duty_cycle;//duty cycle of square wave (1-9 -> 10%->90%)
uint16_t frequency;//the frequency of the output wave in Hz
uint8_t is_ready;


/**
 * sets the time interval for the CCR1 in order to get the
 * time is in us.
 *
 */
void set_timer_fg(uint16_t time)
{
    //do some calculation in here to determine what value to put into the CCR1
    uint32_t beb = time*DCO_SPEED;//calculates the CCRO time that is required

    TIMER_A0->CTL |=
            TIMER_A_CTL_SSEL__SMCLK | // sets timer's source as SMCLK
            TIMER_A_CTL_MC__CONTINUOUS; // sets timer to CONTINUOUS mode

    TIMER_A0->CCTL[0] = TIMER_A_CCTLN_CCIE; // TACCR0 interrupt enable
    TIMER_A0->CCR[0] = beb;
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
    is_ready = 0;

}

/**
 * returns the value of the sine wave at count
 * TODO: NEED TO REMOVE MAGIC NUMBERS - the 100/19 is
 * what makes the base wave 100MHz
 */
uint16_t get_sine(uint32_t count,uint16_t frequency)
{
    return sine_wave_3v[(((uint16_t)(count*(100/19)))
            *(frequency/100))
                        %(sizeof(sine_wave_3v)/sizeof(uint16_t))];//mod by the number of elements in array
}

uint16_t get_square(uint32_t sq_count, uint16_t sq_frequency, uint16_t sq_duty_cycle)
{
    return square_wave[(sq_count%(sizeof(square_wave)/sizeof(uint16_t)))];
}

/**
 * using function pointers - probably not and the global variables that are
 * set at the head of this file, this will return the correct
 * value for the count that we are at.
 */
uint16_t get_value(uint32_t count,uint8_t wave_type)
{
    switch(wave_type)
    {
    case SINE_WAVE:
        return get_sine(count,frequency);
    case SQUARE_WAVE:
        return get_square(count, frequency,duty_cycle);
    default:
        return get_sine(count,frequency);
    }
}

/**
 * our function generator's main.
 */
void main_fg(void)
{
    P6->DIR|=BIT0;
    setup_fg();
    wave_type = SINE_WAVE;
    frequency = 100;
    uint16_t value = 0;
    uint32_t count = 0;
    while(1)
    {
        while(!is_ready);
        value = get_sine(count++,100);
        write_DAC(value);
        is_ready = 0;
    }
}

/**
 * for every time period SAMPLE_TIME, then
 */
void TA0_0_IRQHandler(void)
{
    P6->OUT^=BIT0;
    TIMER_A0->CCTL[0] &= ~TIMER_A_CCTLN_CCIFG;//clear flag
    is_ready = 1;
    TIMER_A0->CCR[0]+=SAMPLE_TIME;//go for next sample time
}
