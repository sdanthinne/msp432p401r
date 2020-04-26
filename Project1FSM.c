/*
 * Project1FSM.c
 *
 *  Created on: Apr 26, 2020
 *      Author: Alexander
 */
#include "Project1FSM.h"

uint8_t init_state(Sys* sys,uint8_t keypress)
{
    //setup(sys);
    set_address_counter_4(0,0);
    clear_display_4();
    sys->current_user_index=0;
    sys->letter_counter=0;
    sys->number_counter=0;
    write_string_4("EnterUser: ");

    return READ_USERNAME_STATE;
}

uint32_t pow(uint8_t base, uint8_t exp)
{
    uint32_t result = 1;
    while (exp)
    {
        if (exp & 1)
            result *= base;
        exp >>= 1;
        base *= base;
    }
    return result;
}

uint8_t check_user_exists(Sys *sys)
{
    uint8_t i;
    for(i = 0; i < sys->user_count; i++) // Iterate through users to find a match
    {
        uint8_t j;
        for(j = 0; j< 4; j++)
        {
            if((sys->users[i].username[j] != sys->curr_user[j]))
            {
               break;   // If the name is not a match. Go to next username
            }
            if(j==3)
            {
                sys->current_user_index = i;
                return READ_PASSWORD_STATE; // Username match go to password state
            }
        }
    }
    return INCORRECT_STATE; // If there is no username match go to incorrect username state
}

uint8_t read_username_state(Sys *sys, uint8_t key_press)
{
    uint8_t curr_press = 0;
    //uint8_t old_press = get_key_pressed();

    //while( (curr_press=update_key_press(curr_press))== 0); //if the key is not the same one as before
    if(sys->letter_counter == 4) // Check to see if username exists if we get to the last letter
    {
       sys->prev_state = READ_USERNAME_STATE;
       sys->letter_counter = 0;
       return check_user_exists(sys);  // Goes to password if user exits. Otherwise byebye
    }

    curr_press=update_key_press(curr_press);

    sys->prev_state = READ_USERNAME_STATE;
    sys->curr_input = curr_press;

    if(sys->curr_input != KEY_POUND && sys->curr_input != KEY_STAR && sys->curr_input != 0) // If key press is a number
    {

        return PROCESS_NUMBER_STATE; // Go to process number state
    }

    return READ_USERNAME_STATE; // If  other button is pressed default back to same state

}




uint8_t process_number_state(Sys *sys, uint8_t key_press)
{

    set_address_counter_4(1, sys->number_counter); // Sets address counter to write newest number to LCD

    write_key_to_LCD(sys->curr_input); // Write newest number to LCD

    if (sys->prev_state == READ_USERNAME_STATE) {

        // Build up ASCII code for character
        uint8_t number_pressed = get_number_pressed(sys->curr_input);
        sys->curr_user[sys->letter_counter] += (number_pressed) * pow(10, 2-(sys->number_counter));
        ++(sys->number_counter);

        sys->prev_state = PROCESS_NUMBER_STATE; // To remember where you came from
        if (sys->number_counter < 3)
        {
            return READ_USERNAME_STATE; // Go here if letter isn't complete
        }

        return PROCESS_ASCII_STATE; // Go here if letter is complete
    }
    ++(sys->number_counter);
    sys->prev_state = PROCESS_NUMBER_STATE; // To remember where you came from
    return READ_PASSWORD_STATE; // Go here if writing password
}

uint8_t process_ascii_state(Sys *sys, uint8_t key_press)
{
    sys->number_counter = 0; // reset number counter for next ascii letter

    set_address_counter_4(0, sys->letter_counter+10); // write letter passed prompt
    write_char_4(sys->curr_user[sys->letter_counter]); // Write current letter of username to LCD
    ++(sys->letter_counter); // Letter has been added to screen

    set_address_counter_4(1, 0);
    write_string_4("    ");
    set_address_counter_4(1, 0);
    sys->prev_state = PROCESS_ASCII_STATE; // To remember where you came from

    return READ_USERNAME_STATE; // Go here when letter has been written to screen
}


/**
 * compares passwords of length 5 at one and two
 */
uint8_t compare_password(uint8_t * one,uint8_t *two)
{
    uint8_t i=0;
    for(i=0;i<=4;i++)
    {
        if(one[i]!=two[i])
            return 0;
    }
    return 1;
}
/**
 * reads in the pressed keypad button and stores it in the curr_pin
 * once the curr_pin is full, then
 */
uint8_t read_password_state(Sys* sys,uint8_t keypress)
{

    uint8_t curr_press = 0;
    if(sys->prev_state==READ_USERNAME_STATE||sys->prev_state==INCORRECT_STATE)
    {
            clear_display_4();
            set_address_counter_4(0,0);
            write_string_4("password: ");//writes "password:" to the top row of the LCD
            sys->prev_state=READ_PASSWORD_STATE;
    }

    curr_press=update_key_press(curr_press);
    sys->curr_input = curr_press;

    sys->curr_pin[sys->number_counter] = get_number_pressed(sys->curr_input);//store the pressed number into the current pin
    sys->prev_state=READ_PASSWORD_STATE;//sets the previous state

    if(sys->number_counter<4)
    {
        return PROCESS_NUMBER_STATE;//go to the process number state
    }
    if(sys->number_counter==4)
    {
        sys->number_counter=0;
        if(compare_password(
                sys->curr_pin,
                sys->users[sys->current_user_index].password))
        {
            //authenticated successfully
            return LOGGED_IN_STATE;
        }
        else
        {
            return INCORRECT_STATE;
        }
    }
    return PROCESS_NUMBER_STATE;

}

uint8_t incorrect_state(Sys* sys,uint8_t keypress)
{
    uint8_t i;
    clear_display_4();
    write_string_4("get out that's wrong");
    delay_us(3000000);//delays 3 seconds

    for (i = 0; i < 4; i++) { // intialize current user array values to 0
            sys->curr_user[i] = 0;
    }

    if(sys->prev_state==READ_USERNAME_STATE)
    {
        sys->prev_state = INCORRECT_STATE;
        return INIT_STATE;
    }
    if(sys->prev_state==READ_PASSWORD_STATE)
    {
        sys->prev_state = INCORRECT_STATE;
        return READ_PASSWORD_STATE;//go back to the password state if it was wrong
    }
    return INCORRECT_STATE;
}

uint8_t logged_in_state(Sys* sys,uint8_t keypress)
{
    if (sys->prev_state != LOGGED_IN_STATE)
    {
        clear_display_4();
        write_string_4("HlloTher");
        set_address_counter_4(1,0);//newline
        write_string_4("GnrlKenobi");
    }
    sys->prev_state=LOGGED_IN_STATE;
    if(keypress==KEY_STAR)
    {
        return INIT_STATE;
    }
    else
    {
        return LOGGED_IN_STATE;
    }
}

/**
 * sets up the sys to have the sudo user
 */
void setup(Sys * sys)
{
    uint8_t i;
    uint8_t temp[5] = {8,0,0,8,5};
    setup_lcd_4();
    setup_keypad();
    clear_display_4();
    memcpy(sys->users[0].username,"sudo",sizeof(char)*4);
    memcpy(sys->users[0].password,temp,sizeof(uint8_t)*5);

    for (i = 0; i < 4; i++) { // intialize current user array values to 0
        sys->curr_user[i] = 0;
    }

    sys->user_count=1;
    sys->current_user_index=0;
    sys->letter_counter=0;
    sys->number_counter=0;
}
