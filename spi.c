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
    //need to configure ports
    P4->DIR|=BIT1;//set to output
    P4->SEL0&=~BIT1;//set to GPIO
    P4->SEL1&=~BIT1;


    P1->SEL0|=BIT6;
    P1->SEL1&=~BIT6;//set to primary function (UCB0SIMO)

    P1->SEL0|=BIT5;
    P1->SEL1&=~BIT5;//set to primary function (UCB0CLK)


    NVIC->ISER[0] |= (1<<(31&EUSCIB0_IRQn));//ENABLE INTERRUPTS !
    EUSCI_B0_SPI -> IE = UCTXIE;// ENABLE INTERRUPTS WITHIN SPI PROTOCOL!

    EUSCI_B0_SPI->CTLW0 |= BIT0; //enable editing of settings/halt the data transmission
    EUSCI_B0_SPI->CTLW0 |= (
            EUSCI_B_CTLW0_MSB | //msb written first
            EUSCI_B_CTLW0_MST | //enable master mode
            EUSCI_B_CTLW0_SYNC | //enable synchronous mode
            EUSCI_B_CTLW0_SSEL__SMCLK | // set to use the smclk
            EUSCI_B_CTLW0_MODE1); //3 pin spi

    EUSCI_B0_SPI->CTLW0 &= ~(
            EUSCI_B_CTLW0_CKPL | //set to be active high
            EUSCI_B_CTLW0_SEVENBIT | //set to 8 bit mode
            EUSCI_B_CTLW0_MODE0 //3 pin spi
            );

    EUSCI_B0_SPI->CTLW0 &= ~BIT0;//this turned off the settings
}


/**
 * this writes a byte over spi to eusci_b
 */
void write_byte_b0(uint8_t byte)
{
    P4->OUT|=BIT1;//set the CS to high
    EUSCI_B0_SPI -> TXBUF = byte; //put in the data

}

void write_byte_b0_cont(uint8_t byte)
{
    EUSCI_B0_SPI -> TXBUF = byte;
}

void EUSCIB0_IRQHandler()
{
    P4->OUT&=~BIT1;//set the cs to low
    EUSCI_B0_SPI->IFG&=~BIT1;//reset flag?

}

