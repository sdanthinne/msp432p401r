/*
 * main.c
 *
 *  Created on: May 18, 2020
 *      Authora: aknapen, crapp, sdanthinne
 */

#include "msp.h"
#include "uart.h"

void main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer

    setup_uart(); // Configure use of UART

    // write to buffer somehow 8 bits
    // wait for interrupt flag to go high (buffer is empty)
    // repeat
    while (1)
    {
        EUSCI_A0->TXBUF = 0x42; // first half of 'A'
        while (EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG); // wait until TXBUF empty
        EUSCI_A0->TXBUF = 0; // second half of 'A'
        while (EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG); // wait until TXBUF empty
    }
}
