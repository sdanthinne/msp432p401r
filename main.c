/*
 * main.c
 *
 *  Created on: May 18, 2020
 *      Authors: aknapen, crapp, sdanthinne
 */

#include "msp.h"
#include "uart.h"
#include "DAC.h"
#include "arduino.h"


void main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer

    setup_DAC(); // configure use of SPI with the DAC
    setup_uart(); // Configure use of UART
    setup_bt_uart(); // Configure use of UART
    setup_bluetooth();
    setup_arduino();
    write_bt_command("TYPE2");
    send_number(150.999);
    while (1)
    {
//        write_string_uart("A");
    }
}
