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
void setup_arduino()
{
    setup_SPI_b0();
    set_SM_DCO();

}

/**
 * sends a 32 bit float LSB first
 */
void send_number(float number)
{
    union float_split{
        uint8_t _byte[4];
        float _float;
        uint32_t _int;
    } u;
    u._float = number;//adding the number to the union
    uint8_t i;
    for(i=0;i<FLOAT_SIZE;i++)
    {
        write_byte_b0(u._byte[i]);
    }
}

