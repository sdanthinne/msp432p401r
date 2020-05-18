/*
 * uart.c
 *
 *  Created on: May 18, 2020
 *      Author: Alexander
 */
#include "msp.h"

void setup_uart()
{
    EUSCI_A0->CTLW0 |= EUSCI_A_CTLW0_SWRST; // software reset enable

    EUSCI_A0->CTLW0 |= EUSCI_A_CTLW0_SSEL__SMCLK | // select SMCLK as souce
                       EUSCI_A_CTLW0_MODE_0 | // use UART mode
                       EUSCI_A_CTLW0_PAR | // use even parity
                       EUSCI_A_CTLW0_PEN; // enable parity

}
