/*
 * main.c
 *
 *  Created on: May 18, 2020
 *      Authors: aknapen, crapp, sdanthinne
 */

#include "msp.h"
#include "uart.h"
#include "bluetooth.h"
#include "DAC.h"
#include "arduino.h"
#include "ADC.h"
#include "delay.h"


void main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer
    uint16_t adc_val;
    uint32_t calibrated_voltage;

    P5->SEL0 |= BIT5;
    P5->SEL1 |= BIT5; // P5.5 becomes A0
    set_SM_DCO();
    setup_uart();
    setup_ADC();

    P1->SEL0 &= ~BIT0;
    P1->SEL1 &= ~BIT0;
    P1->DIR |= BIT0;
    P1->OUT &= ~BIT0;

    while(1)
    {
        if(adc_flag)
        {
            delay_us(1000000); // 1 s delay
            calibrated_voltage = calibrated_adc_voltage();
            write_UART_32bitnum(calibrated_voltage);
            adc_flag = 0;
            ADC14->CTL0 |= ADC14_CTL0_SC;
        }
    }

}
