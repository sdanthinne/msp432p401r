/*
 * uart.c
 *
 *  Created on: May 30, 2020
 *      Author: crapp
 */
#include "bluetooth.h"

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

    EUSCI_A2->CTLW0 |= EUSCI_A_CTLW0_SSEL__SMCLK | // select SMCLK as source
                       EUSCI_A_CTLW0_PEN | // enable parity
                       EUSCI_A_CTLW0_PAR; // use even parity


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
    is_awake=0;
    P4->DIR |= BIT1;
    P4->SEL0 &= ~BIT1;
    P4->SEL1 &= ~BIT1;
    P4->OUT &= BIT1;
//    write_string_uart("AT\r\n");
//    write_string_uart("AT+BAUD\r\n\n");
//    write_bt_command("BAUD");
    write_bt_command("VERSION");
//    EUSCI_A0->TXBUF = " ";
//    EUSCI_A0->TXBUF = " ";
//    write_bt_command("BAUD");
//    write_bt_command("UARTMODE");
//        while(!is_awake);
//    write_string_uart("i am ironman i am ironman i am ironman i am ironman"
//    "i am ironman i am ironman i am ironman i am ironman i am ironman i am ironman\r\n"); // Necessary to send a long string to wake up from sleep mode
//
//    write_bt_command("BAUD[0]");    // Sets the Baud rate to 9600


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
    write_string_uart("AT+");
    write_string_uart(str);
    write_string_uart("\r\n");

}


void EUSCIA2_IRQHandler(void)
{
    uint8_t read_val = EUSCI_A2->RXBUF;
    EUSCI_A0->TXBUF = read_val;
//    EUSCI_A2->IFG &= ~(EUSCI_A_IFG_RXIFG);
}
