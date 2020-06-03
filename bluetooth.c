/*
 * uart.c
 *
 *  Created on: May 30, 2020
 *      Author: crapp
 */
#include "bluetooth.h"

uint8_t response_received;

void setup_bt_uart()
{
    // Configure Tx and Rx pins
    P3->SEL0 |= BIT2;
    P3->SEL1 &= ~BIT2; // configure P3.2 as UCA2RXD

    P3->SEL0 |= BIT3;
    P3->SEL1 &= ~BIT3;// configure P3.3 as UCA2TXD


    EUSCI_A2->CTLW0 |= EUSCI_A_CTLW0_SWRST; // enable software reset

    // Configure UART settings
    EUSCI_A2->CTLW0 &= ~(EUSCI_A_CTLW0_MODE_MASK | // select UART mode
                         EUSCI_A_CTLW0_SEVENBIT | // use 8-bit character length
                         EUSCI_A_CTLW0_SPB | // use one stop bit
                         EUSCI_A_CTLW0_MSB | // send LSB first
                         EUSCI_A_CTLW0_SYNC | // use asynchronous mode
                         EUSCI_A_CTLW0_TXBRK | // next frame is not a  transmit break
                         EUSCI_A_CTLW0_TXADDR); // next frame is data

    EUSCI_A2->CTLW0 |= EUSCI_A_CTLW0_SSEL__SMCLK; // select SMCLK as source
                       //EUSCI_A_CTLW0_PEN | // enable parity
                       //EUSCI_A_CTLW0_PAR; // use even parity


    // Baud rate configuration for 9600 bps with a 12 MHz clock

    EUSCI_A2->BRW = 78; // set clock prescaler for baud rate generation

    EUSCI_A2->MCTLW |= EUSCI_A_MCTLW_OS16; // enable oversampling mode
    EUSCI_A2->MCTLW |= UCBRF__2; // First module stage select for modulation pattern
    EUSCI_A2->MCTLW &= ~EUSCI_A_MCTLW_BRS_MASK; // clears second modulation stage select

    EUSCI_A2->CTLW0 &= ~EUSCI_A_CTLW0_SWRST; // disable software reset


    EUSCI_A2->IE |= EUSCI_A_IE_RXIE; // enable receive interrupts
    NVIC->ISER[0] = 1 << (EUSCIA2_IRQn & 0x1F); //enable interrupts for EUSCI_A2
    __enable_irq(); // enable global interrupts

}

void setup_bluetooth()
{
    query_version();
}

void query_mac_address()
{
    write_bt_command("LADDR");
}

void query_version()
{
    write_bt_command("VERSION");
}

void query_baud()
{
    write_bt_command("BAUD");
}

void disconnect_bt()
{
    write_bt_command("DISC");
}


void write_string_uart(char *str)
{
    while(*str!='\0')
    {
        EUSCI_A2->TXBUF = *str;
        while(!(EUSCI_A2->IFG & UCTXIFG)); // Wait for txbuf to be ready to receive another character

        EUSCI_A0->TXBUF = *str;
        while(!(EUSCI_A0->IFG & UCTXIFG)); // Wait for txbuf to be ready to receive another character

        str+=sizeof(char);
    }
}

void write_bt_command(char * str)
{
    response_received = 0;
    write_string_uart("AT+");
    write_string_uart(str);
    write_string_uart("\r\n");
    while(!response_received);
}


void EUSCIA2_IRQHandler(void)
{
    uint8_t read_val = EUSCI_A2->RXBUF;
    EUSCI_A0->TXBUF = read_val;
    if(read_val =='\n')
    {
        response_received = 1;
    }

}
