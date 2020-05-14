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
 *
 *IMPORTANT when getting the settings from the ISR with the keypad, we should maybe
 *IMPORTANT stop the interrupts of the DAC output? Might help with interrupt priority
 *IMPORTANT I think that interrupts from timer precede over port interrupts.
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
    NVIC->IP[2] |= BIT0; // assigns interrupt priority 1 (second highest) to TimerA0 in NVIC
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
 * returns the value of the sine wave at count
 * TODO: NEED TO REMOVE MAGIC NUMBERS - the 100/19 is
 * what makes the base wave 100MHz - currently incorrect
 */
uint16_t get_sine(uint32_t sin_count,uint16_t sin_frequency)
{
    uint16_t max_count = (INTERRUPT_FREQUENCY / sin_frequency);
    uint16_t normalized_count = sin_count % max_count;
    uint16_t array_val = (512 * normalized_count) / (max_count);
    return sine_wave_3v[array_val];
}

/**
 * returns the correct value for the square wave with duty cycle
 */
uint16_t get_square(uint32_t sq_count, uint16_t sq_frequency, uint16_t sq_duty_cycle)
{
    return square_wave[(sq_count % (INTERRUPT_FREQUENCY/sq_frequency))     // Creates an adjust sq count normalized to a maximum determined by frequency of interrupts and square wave
                        < ((INTERRUPT_FREQUENCY/(sq_frequency)+1) *(sq_duty_cycle)/10)];   // checks to see if normalized square count is less than the maximum square count divided by duty cycle
}

/**
 * returns the value of the sine wave at count
 * TODO: NEED TO REMOVE MAGIC NUMBERS - the 100/19 is
 * what makes the base wave 100MHz - currently incorrect
 */
uint16_t get_triangle(uint32_t tri_count,uint16_t tri_frequency)
{
    uint16_t max_count = (INTERRUPT_FREQUENCY / tri_frequency);
    uint16_t normalized_count = tri_count % max_count;
    uint16_t array_val = (512 * normalized_count) / (max_count);
    return triangle_wave_3v[array_val];
}


/**
 * generates a sawtooth wave based on the counter
 */
uint16_t get_sawtooth(uint32_t tri_count,uint16_t tri_frequency)
{
    uint16_t max_count = (INTERRUPT_FREQUENCY / tri_frequency);
    uint16_t normalized_count = tri_count % max_count;
    uint16_t saw_val = (930 * normalized_count) / (max_count);
    return saw_val;
}

/**
 * using function pointers - probably not and the global variables that are
 * set at the head of this file, this will return the correct
 * value for the count that we are at.
 * currently this case statement is a little slow. -
 * noticable in terms of waveform
 * if block is marginally faster
 */
uint16_t get_value(uint32_t count,uint8_t wave_type)
{
    uint16_t out_value = 0;

    if(wave_type & SQUARE_WAVE)
    {
        out_value+= get_square(count, frequency,duty_cycle);
    }
    if(wave_type & SAWTOOTH_WAVE)
    {
        out_value+= get_sawtooth(count,frequency);
    }
    if(wave_type & SINE_WAVE)
    {
        out_value+= get_sine(count,frequency);
    }
    if(wave_type & TRIANGLE_WAVE)
    {
        out_value+= get_triangle(count,frequency);
    }
    return out_value/wave_count;
//    switch(wave_type)
//    {
//    case SINE_WAVE:
//        return get_sine(count,frequency);
//    case SQUARE_WAVE:
//        return get_square(count, frequency,duty_cycle);
//    case SAWTOOTH_WAVE:
//        return get_sawtooth(count,frequency);
//    default:
//        return get_sine(count,frequency);
//    }
}

/**
 * our function generator's main.
 */
void main_fg(void)
{
    uint16_t value = 0;
    uint32_t count = 0;

    P6->DIR|=BIT0; //FOR DIAGNOSTICS/testing
    setup_fg();//run setup
    wave_type = SQUARE_WAVE;//select the default waveform to output
    wave_count = 1;
    duty_cycle = 9;
    frequency = 100;//set the frequency

    while(1)
    {
        value = get_value(count++,wave_type);//get the value
        //P6->OUT&=~BIT0;
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
    //P6->OUT^=BIT0;//WARNING: ADDING THIS LINE SIGNIFICANTLY INCREASES PERIOD
    TIMER_A0->CCTL[0] &= ~TIMER_A_CCTLN_CCIFG;//clear flag
    is_ready = 1;
    TIMER_A0->CCR[0]+=320;//go for next sample time
}
