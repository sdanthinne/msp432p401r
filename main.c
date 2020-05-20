/*
 * main.c
 *
 *  Created on: May 18, 2020
 *      Authora: aknapen, crapp, sdanthinne
 */

#include "msp.h"

void main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer

    setup_uart(); // Configure use of UART
}
