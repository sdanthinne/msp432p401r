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
    NVIC->ISER[0] |= (1<<(31&EUSCIB0_IRQn));//ENABLE INTERRUPTS!
    EUSCI_B0_SPI -> IE[UCTXIE] = 1;

    EUSCI_B0_SPI->CTLW0 |= BIT1; //enable editing of settings/halt the data transmission
    EUSCI_B0_SPI->CTLW0 |= (
            EUSCI_B_CTLW0_MSB | //msb written first
            EUSCI_B_CTLW0_MST | //enable master mode
            EUSCI_B_CTLW0_SYNC | //enable synchronous mode
            EUSCI_B_CTLW0_SSEL__SMCLK); // set to use the smclk

    EUSCI_B0_SPI->CTLW0 &= ~(
            EUSCI_B_CTLW0_CKPL | //set to be active high
            EUSCI_B_CTLW0_SEVENBIT | //set to 8 bit mode
            EUSCI_B_CTLW0_MODE_0 //3 pin spi
            );
}


/**
 * this writes a byte over spi to eusci_b
 */
void write_byte_b0(uint8_t byte)
{

}

