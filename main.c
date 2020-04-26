#include "Project1FSM.h"

State FSM[7] = {//state function,ns0,ns1.ns2,ns3...
                 {init_state,{INIT_STATE_LOC,READ_USERNAME_STATE_LOC,PROCESS_NUMBER_STATE_LOC,PROCESS_ASCII_STATE_LOC,READ_PASSWORD_STATE_LOC,INCORRECT_STATE_LOC,LOGGED_IN_STATE_LOC}},
                 {read_username_state,{INIT_STATE_LOC,READ_USERNAME_STATE_LOC,PROCESS_NUMBER_STATE_LOC,PROCESS_ASCII_STATE_LOC,READ_PASSWORD_STATE_LOC,INCORRECT_STATE_LOC,LOGGED_IN_STATE_LOC}},
                 {process_number_state,{INIT_STATE_LOC,READ_USERNAME_STATE_LOC,PROCESS_NUMBER_STATE_LOC,PROCESS_ASCII_STATE_LOC,READ_PASSWORD_STATE_LOC,INCORRECT_STATE_LOC,LOGGED_IN_STATE_LOC}},
                 {process_ascii_state,{INIT_STATE_LOC,READ_USERNAME_STATE_LOC,PROCESS_NUMBER_STATE_LOC,PROCESS_ASCII_STATE_LOC,READ_PASSWORD_STATE_LOC,INCORRECT_STATE_LOC,LOGGED_IN_STATE_LOC}},
                 {read_password_state,{INIT_STATE_LOC,READ_USERNAME_STATE_LOC,PROCESS_NUMBER_STATE_LOC,PROCESS_ASCII_STATE_LOC,READ_PASSWORD_STATE_LOC,INCORRECT_STATE_LOC,LOGGED_IN_STATE_LOC}},
                 {incorrect_state,{INIT_STATE_LOC,READ_USERNAME_STATE_LOC,PROCESS_NUMBER_STATE_LOC,PROCESS_ASCII_STATE_LOC,READ_PASSWORD_STATE_LOC,INCORRECT_STATE_LOC,LOGGED_IN_STATE_LOC}},
                 {logged_in_state,{INIT_STATE_LOC,READ_USERNAME_STATE_LOC,PROCESS_NUMBER_STATE_LOC,PROCESS_ASCII_STATE_LOC,READ_PASSWORD_STATE_LOC,INCORRECT_STATE_LOC,LOGGED_IN_STATE_LOC}}
};



void main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer
    //uint8_t usercount=0;

    State * curr_state;
    uint8_t state_input;
    uint8_t keypress=0;
    Sys sys;
    setup(&sys);
    curr_state = INIT_STATE_LOC;//enter username
    state_input = (curr_state->state_function)(&sys,keypress);
    curr_state = curr_state->next[state_input];

    delay_us(50000);
    while(1)
    {
        keypress = get_key_pressed();    //get ASCII of the key pressed
        state_input = (curr_state->state_function)(&sys,keypress);//calls respective function to the state that we are in
        curr_state = curr_state->next[state_input];//sets the next state based on the prev state
    }


}
