/*
 * uart.c
 *
 *  Created on: May 30, 2020
 *      Author: aknapen, crapp, sdanthinne
 */
#include <stdio.h>
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
    data_counter =0;
    data_in._int =0;
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


void write_string_bt(char *str)
{
    while(*str!='\0')
    {
        EUSCI_A2->TXBUF = *str;
        while(!(EUSCI_A2->IFG & UCTXIFG)); // Wait for txbuf to be ready to receive another character
        str+=sizeof(char);
    }
}

void write_bt_command(char * str)
{
    bt_str_rec = 0;
    write_string_bt("AT+");
    write_string_bt(str);
    write_string_bt("\r\n");
    while(!bt_str_rec);
}

void read_bt_string()
{
    bt_str_rec = 0;
    bt_byte_rec = 0;
    uint8_t byte_count = 0;

    while(!bt_str_rec)  // While not a string
    {
        while(!bt_byte_rec);    // Wait for byte received
        bt_data[byte_count] = bt_byte;  // Add the recent data to the array
        byte_count++;
        //EUSCI_A0->TXBUF = bt_byte;
        bt_byte_rec = 0;

    }
    bt_data[byte_count] = bt_byte;  // Write the final character
    bt_data[byte_count+1] = 0; // Null terminate the string
    write_UART_string(bt_data);
}

void EUSCIA2_IRQHandler(void)
{
    uint8_t read_val = EUSCI_A2->RXBUF;
    //EUSCI_A0->TXBUF = read_val;
    //write_byte_b0(read_val);

    data_in._byte[data_counter%4]=read_val;
    if(data_counter%4==3)
    {
        //char string[20];
        //sprintf(string,"%u\n",data_in._int/1024);

        //__disable_irq();
        send_number(data_in._int);
//        write_byte_b0(local_var._byte[3]);
//        write_byte_b0(local_var._byte[2]);
//        write_byte_b0(local_var._byte[1]);
//        write_byte_b0(local_var._byte[0]);
        //__enable_irq();
        //write_UART_string(string);
        data_in._int=0;
    }

    data_counter++;

    bt_byte = read_val;
    bt_byte_rec = 1;
    if(read_val =='\n')
    {
        bt_str_rec = 1;
    }

}
