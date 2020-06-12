/*
 * main.c
 *
 *  Created on: May 18, 2020
 *      Authors: aknapen, crapp, sdanthinne
 */

#include "msp.h"
#include "uart.h"
#include "ADC.h"
#include "delay.h"


void main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer
    uint16_t adc_val;
    uint32_t calibrated_voltage;

    P5->SEL0 |= BIT5;
    P5->SEL1 |= BIT5; // P5.5 becomes A0
    set_SM_DCO();   // Set the SMClock to 12Mhz
    setup_uart();   // Set up UART functionality on EUSCIA0
    setup_ADC();    // Setup the ADC to use P5.5 / A0

    while(1)
    {
        if(adc_flag)    // When the adc has updated
        {
            delay_us(1000000); // 1 s delay
            calibrated_voltage = calibrated_adc_voltage();
            write_UART_32bitnum(calibrated_voltage);//Write voltage to terminal
            adc_flag = 0;   // Indicate ready for new value
            ADC14->CTL0 |= ADC14_CTL0_SC;   // Tell adc to start reading
        }
    }

}
