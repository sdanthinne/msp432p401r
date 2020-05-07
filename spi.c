#include "spi.h"
/*
 * spi.c
 *
 *  Created on: May 6, 2020
 *      Author: sdanthinne
 */

/**
 * sets upthe spi interface with the board on eUSCI_B0
 */
void setup_SPI_b0()
{
    EUSCI_B0_SPI->CTLW0 |= BIT0; //enable editing of settings/halt the data transmission

    P4->DIR|=BIT1;//set to output
    P4->SEL0&=~(BIT1);//set to GPIO
    P4->SEL1&=~(BIT1);
    P4->OUT&=~(BIT1);

    P1->DIR|=BIT6|BIT5;
    P1->SEL0|=BIT6 | BIT5;//set to primary function (UCB0SIMO)(UCB0CLK)

    EUSCI_B0_SPI->CTLW0 |= (
            EUSCI_B_CTLW0_MSB | //msb written first
            EUSCI_B_CTLW0_MST | //enable master mode
            EUSCI_B_CTLW0_SYNC | //enable synchronous mode
            EUSCI_B_CTLW0_SSEL__SMCLK | // set to use the smclk
            EUSCI_B_CTLW0_CKPL); //3 pin spi

    EUSCI_B0_SPI->BRW = 0x00;//set the bitrate to go at SMCLK (0)

    EUSCI_B0_SPI->CTLW0 &= ~BIT0;//re-enable data transmission
}

/**
 * this writes a byte over spi to eusci_b
 */
void write_byte_b0(uint8_t byte)
{
    //(EUSCI_B0_SPI->IFG&BIT1)
    while((EUSCI_B0_SPI->STATW&EUSCI_B_STATW_SPI_BUSY));//wait for the txbuf to be empty
    EUSCI_B0_SPI -> TXBUF = byte; //put in the data
}

/*
 * not currently necessary
 */
void EUSCIB0_IRQHandler()
{
    if(EUSCI_B0_SPI->IV==0x4)//if the interrupt is of the correct type (TX)
    {
        while((EUSCI_B0_SPI->STATW&EUSCI_B_STATW_SPI_BUSY));//wait for it to finish writing
    }
    EUSCI_B0_SPI->IFG&=~BIT1;//reset flag
}

