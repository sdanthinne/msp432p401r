#include "DAC.h"
#include "DCO.h"
#include "delay.h"
/*
 * DAC.c
 *
 *  Created on: May 7, 2020
 *      Author: sdanthinne
 *      to be used with the MCP4912
 ,*/


/**
 * sets up the dac for write_dac to be able to transmit data
 */
void setup_DAC()
{

    set_SM_DCO();
    setup_SPI_b0();
    P4->OUT|=BIT1;//set cs high for default high
}

/**
 * writes to the MCP4912
 * since data is only 10 bits, we assume that only the lower 10 bits
 * of bytes are used.
 */
void write_DAC(uint16_t bytes)
{
    bytes = bytes & (0b1111111111);//select only the 10 bits

    P4->OUT&=~BIT1;//set the cs to low

    write_byte_b0(0x70|(bytes>>6));//put the data into the txbuf with the correct settings
//
    write_byte_b0((bytes<<2));//write the next half of the byte

    while((EUSCI_B0_SPI->STATW & EUSCI_B_STATW_SPI_BUSY));//wait for the entire signal to be written

    P4->OUT|=BIT1;//set cs high again after signal has been written

}
