/**
 * Main. used to call multiple projects
 */
#include "Project1FSM.h"
void main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer
    run_P1();//runs project 1
}
