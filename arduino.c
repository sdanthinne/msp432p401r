#include "msp.h"
#include "arduino.h"
#include "spi.h"
#include "DCO.h"
/*
 * arduino.c
 *
 *  Created on: Jun 2, 2020
 *      Author: sdanthinne
 */

/**
 * initializes the communication between the arduino and the MSP
 */
void init_arduino()
{
    setup_SPI_b0();
    set_SM_DCO();

}

/**
 * sends a 32 bit float
 */
void send_number(float number)
{
    u.f_val = number;//adding the number to the union
    uint8_t i;
    for(i=FLOAT_SIZE+1;i!=0;i--)
    {
        write_byte_b0(u.split_val[i-1]);
    }
}

