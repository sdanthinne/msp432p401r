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
    uint16_t read_val = 0;

    P1->DIR |= BIT0; // set P1.0 output
    P1->SEL0 &= ~BIT0;
    P1->SEL1 &= ~BIT0; // select GPIO for P1.0
    P1->OUT &= ~BIT0; // turn LED off initially

    while (1)
    {
        while (~(EUSCI_A0->IFG & EUSCI_A_IFG_RXIFG) & EUSCI_A_IFG_RXIFG); // wait until RXBUF is filled
        //EUSCI_A0->TXBUF = 0x41; // transmit 'A'
        read_val = EUSCI_A0->RXBUF;
        if (read_val == 0x41) // if an A is read
        {
            P1->OUT ^= BIT0; // toggle LED
        }
    }
}
