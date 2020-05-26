/*
 * uart.c
 *
 *  Created on: May 18, 2020
 *      Author: aknapen, crapp, sdanthinne
 */
#include "msp.h"
#include "DCO.h"
#include "uart.h"

void setup_uart()
{
    set_SM_DCO(); // Configure DCO and SMCLK settings

    EUSCI_A0->CTLW0 |= EUSCI_A_CTLW0_SWRST; // enable software reset

    // Configure UART settings
    EUSCI_A0->CTLW0 &= ~EUSCI_A_CTLW0_MODE_MASK; // select UART mode
    EUSCI_A0->CTLW0 &= ~EUSCI_A_CTLW0_SEVENBIT; // use 8-bit character length
    EUSCI_A0->CTLW0 &= ~EUSCI_A_CTLW0_SPB; // use one stop bit
    EUSCI_A0->CTLW0 &= ~EUSCI_A_CTLW0_MSB; // send LSB first
    EUSCI_A0->CTLW0 &= ~EUSCI_A_CTLW0_SYNC; // use asynchronous mode
    EUSCI_A0->CTLW0 &= ~EUSCI_A_CTLW0_TXBRK; // next frame is not a  transmit break
    EUSCI_A0->CTLW0 &= ~EUSCI_A_CTLW0_TXADDR; // next frame is data

    EUSCI_A0->CTLW0 |= EUSCI_A_CTLW0_SSEL__SMCLK | // select SMCLK as source
                       EUSCI_A_CTLW0_PEN | // enable parity
                       EUSCI_A_CTLW0_PAR; // use even parity

    EUSCI_A0->CTLW0 &= ~EUSCI_A_CTLW0_SPB; // use one stop bit


    // Baud rate configuration for 9600 bps with a 1.5 MHz clock

    EUSCI_A0->BRW = 78; // set clock prescaler for baud rate generation

    EUSCI_A0->MCTLW |= EUSCI_A_MCTLW_OS16; // enable oversampling mode
    EUSCI_A0->MCTLW |= UCBRF__2; // First module stage select for modulation pattern
    EUSCI_A0->MCTLW &= ~EUSCI_A_MCTLW_BRS_MASK; // clears second modulation stage select

    EUSCI_A0->CTLW0 &= ~EUSCI_A_CTLW0_SWRST; // disable software reset
}
