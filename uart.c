/*
 * uart.c
 *
 *  Created on: May 18, 2020
 *      Author: aknapen, crapp, sdanthinne
 */
#include "msp.h"
#include "DCO.h"
#include "uart.h"
#include "keypad.h"



void setup_uart()
{
    // Configure Tx and Rx pins
    P1->SEL0 |= BIT2;
    P1->SEL1 &= ~BIT2; // configure P1.2 as UCA0RXD

    P1->SEL0 |= BIT3;
    P1->SEL1 &= ~BIT3;// configure P1.3 as UCA0TXD


    EUSCI_A0->CTLW0 |= EUSCI_A_CTLW0_SWRST; // enable software reset

    // Configure UART settings
    EUSCI_A0->CTLW0 &= ~(EUSCI_A_CTLW0_MODE_MASK | // select UART mode
                         EUSCI_A_CTLW0_SEVENBIT | // use 8-bit character length
                         EUSCI_A_CTLW0_SPB | // use one stop bit
                         EUSCI_A_CTLW0_MSB | // send LSB first
                         EUSCI_A_CTLW0_SYNC | // use asynchronous mode
                         EUSCI_A_CTLW0_TXBRK | // next frame is not a  transmit break
                         EUSCI_A_CTLW0_TXADDR); // next frame is data

    EUSCI_A0->CTLW0 |= EUSCI_A_CTLW0_SSEL__SMCLK | // select SMCLK as source
                       EUSCI_A_CTLW0_PEN | // enable parity
                       EUSCI_A_CTLW0_PAR; // use even parity


    // Baud rate configuration for 9600 bps with a 12 MHz clock

    EUSCI_A0->BRW = 78; // set clock prescaler for baud rate generation

    EUSCI_A0->MCTLW |= EUSCI_A_MCTLW_OS16; // enable oversampling mode
    EUSCI_A0->MCTLW |= UCBRF__2; // First module stage select for modulation pattern
    EUSCI_A0->MCTLW &= ~EUSCI_A_MCTLW_BRS_MASK; // clears second modulation stage select

    EUSCI_A0->CTLW0 &= ~EUSCI_A_CTLW0_SWRST; // disable software reset


    EUSCI_A0->IE |= EUSCI_A_IE_RXIE; // enable receive interrupts
    NVIC->ISER[0] = 1 << (EUSCIA0_IRQn & 0x1F); //enable interrupts for EUSCI_A0
    __enable_irq(); // enable global interrupts
}

void EUSCIA0_IRQHandler(void)
{
    uint8_t read_val = EUSCI_A0->RXBUF;
    write_byte_b0(read_val);
    EUSCI_A0->TXBUF = read_val;
    //EUSCI_A2->TXBUF = read_val;
}
