#include "spi.h"
/*
 * spi.c
 *
 *  Created on: May 6, 2020
 *      Author: sdanthinne
 */

/**
 * sets up the spi interface with the board on eUSCI_B0
 */
void setup_SPI_b0()
{
    EUSCI_B0_SPI->CTLW0 |= BIT0; //enable editing of settings/halt the data transmission

    P1->DIR|=BIT6|BIT5;
    P1->SEL0|=BIT6 | BIT5;//set to primary function (UCB0SIMO)(UCB0CLK)

    EUSCI_B0_SPI->CTLW0 |= (
            EUSCI_B_CTLW0_MSB | //msb written first
            EUSCI_B_CTLW0_MST | //enable master mode
            EUSCI_B_CTLW0_SYNC | //enable synchronous mode
            EUSCI_B_CTLW0_SSEL__SMCLK | // set to use the smclk
            EUSCI_B_CTLW0_CKPL); //3 pin spi

    EUSCI_B0_SPI->BRW = 0x0060;//set the bitrate to go at SMCLK (0)

    EUSCI_B0_SPI->CTLW0 &= ~BIT0;//re-enable data transmission
}

/**
 * this writes a byte over spi to eusci_b
 */
void write_byte_b0(uint8_t byte)
{
    while ((~(EUSCI_B0_SPI->IFG & BIT1)) & BIT1);
    EUSCI_B0_SPI -> TXBUF = byte; //put in the data to the buffer
}
