/*
 * main.c
 *
 *  Created on: May 18, 2020
 *      Authors: aknapen, crapp, sdanthinne
 */

#include "msp.h"
#include "uart.h"

void main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer
    setup_uart(); // Configure use of UART

    P6->DIR |= BIT4;
    P6->SEL0 &= ~BIT4;
    P6->SEL1 &= ~BIT4;
    P6->OUT |= BIT4;

    while (1)
    {
        while (~(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG) & EUSCI_A_IFG_TXIFG); // wait until TXBUF empty
        EUSCI_A0->TXBUF = 0x42; // first half of 'A'
        while (~(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG) & EUSCI_A_IFG_TXIFG); // wait until TXBUF empty
        EUSCI_A0->TXBUF = 2; // second half of 'A'
        P6->OUT ^= BIT4;
    }
}
