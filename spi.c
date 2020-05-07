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

    //need to configure ports
    P4->DIR|=BIT1 | BIT2;//set to output
    P4->SEL0&=~(BIT1 | BIT2);//set to GPIO
    P4->SEL1&=~(BIT1 | BIT2);
    P4->OUT&=~(BIT1 | BIT2);

    P1->DIR|=BIT6|BIT5;
    P1->SEL0|=BIT6 | BIT5;//set to primary function (UCB0SIMO)(UCB0CLK)


    //__enable_irq(); //enable global interrupts
    //NVIC->ISER[0] |= (1<<(31&(EUSCIB0_IRQn)));//ENABLE INTERRUPTS !
    EUSCI_B0_SPI -> IE |= UCTXIE;// ENABLE INTERRUPTS WITHIN SPI PROTOCOL!


    EUSCI_B0_SPI->CTLW0 |= (
            EUSCI_B_CTLW0_MSB | //msb written first
            EUSCI_B_CTLW0_MST | //enable master mode
            EUSCI_B_CTLW0_SYNC | //enable synchronous mode
            EUSCI_B_CTLW0_SSEL__SMCLK | // set to use the smclk
            EUSCI_B_CTLW0_CKPL); //3 pin spi

//    EUSCI_B0_SPI->CTLW0 &= ~(
//            EUSCI_B_CTLW0_SEVENBIT | //set to 8 bit mode
//            EUSCI_B_CTLW0_MODE0 |//3 pin spi
//            EUSCI_B_CTLW0_SSEL0
//            );
    EUSCI_B0_SPI->BRW = 0x00;//BREW

    EUSCI_B0_SPI->CTLW0 &= ~BIT0;//re-enable data transmission
//    EUSCI_B0_SPI -> IE |= UCTXIE;// ENABLE INTERRUPTS WITHIN SPI PROTOCOL!
//    EUSCI_B0_SPI->IFG &=~BIT1; //clear flags after hard reset
}

/**
 * this writes a byte over spi to eusci_b
 */
void write_byte_b0(uint8_t byte)
{
    //delay_us(200);
    while((EUSCI_B0_SPI->IFG&BIT1));//wait for the txbuf to be empty
    EUSCI_B0_SPI -> TXBUF = byte; //put in the data
}

/**
 * writes to the MCP4912
 */
void write_DAC(uint16_t bytes)
{
    //P4->OUT|=BIT2;//set ldac to high
    EUSCI_B0_SPI->IFG&=~BIT1;

    P4->OUT&=~BIT1;//set the cs to low
    write_byte_b0((uint8_t)(0x30|((bytes>>7)&0xF)));//put the data into the txbuf with the correct settings

    //while();
    //Problem: not finishing writing the above before getting to the rest

    write_byte_b0((uint8_t)bytes);//write the next half of the byte
    while((EUSCI_B0_SPI->STATW&EUSCI_B_STATW_SPI_BUSY));
    P4->OUT|=BIT1;//set cs high

    //below may be unnecessary
    //P4->OUT&=~BIT2;//toggle ldac for a little bit
    //delay_us(100);
    //P4->OUT|=1;//set ldac high
}

/*
 * not currently necessary
 */
void EUSCIB0_IRQHandler()
{
    if(EUSCI_B0_SPI->IV==0x4)//if the interrupt is of the correct type (TX)
    {
        while((EUSCI_B0_SPI->STATW&EUSCI_B_STATW_SPI_BUSY));//wait for it to finish writing
        //P4->OUT&=~BIT1;//set the cs to low
    }
    EUSCI_B0_SPI->IFG&=~BIT1;//reset flag
}

