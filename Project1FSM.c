/*
 * Project1FSM.c
 *
 *  Created on: Apr 26, 2020
 *      Author: sdanthinne,aknapen,crapp
 */
#include "Project1FSM.h"

/**
 * struct to define a user
 * for the system
 */
typedef struct
{
    char username[4];
    uint8_t password[5];
} User;

/**
 * keeps track of:
 *  user table
 *  user count
 *  current user
 *  current pin
 *  current user index
 */
typedef struct
{
    User users[5];
    uint8_t user_count;
    char curr_user[4];
    uint8_t curr_pin[5],current_user_index;
    uint8_t prev_state;
    uint8_t letter_counter;
    uint8_t number_counter;

    uint8_t curr_input;
}Sys;

/**
 * defines a state in the program's FSM
 * with each state containing an associated
 * use function and a list of the possible
 * next states
 */
typedef struct State
{
    uint8_t (*state_function)(Sys*,uint8_t);
    struct State *next[7];
} State;


/**
 * Numbers FSM states for use in the next array
 * of each state.
 */
#define INIT_STATE 0
#define READ_USERNAME_STATE 1
#define PROCESS_NUMBER_STATE 2
#define PROCESS_ASCII_STATE 3
#define READ_PASSWORD_STATE 4
#define INCORRECT_STATE 5
#define LOGGED_IN_STATE 6

/**
 * Defines each state relative to its placement
 * in the FSM array
 */
#define INIT_STATE_LOC &FSM[0]
#define READ_USERNAME_STATE_LOC &FSM[1]
#define PROCESS_NUMBER_STATE_LOC &FSM[2]
#define PROCESS_ASCII_STATE_LOC &FSM[3]
#define READ_PASSWORD_STATE_LOC &FSM[4]
#define INCORRECT_STATE_LOC &FSM[5]
#define LOGGED_IN_STATE_LOC &FSM[6]

/**
 * List of function prototypes for use by
 * the FSM array.
 */
uint8_t init_state(Sys* sys,uint8_t keypress);
uint8_t read_username_state(Sys* sys,uint8_t keypress);
uint8_t process_number_state(Sys* sys,uint8_t keypress);
uint8_t process_ascii_state(Sys* sys,uint8_t keypress);
uint8_t read_password_state(Sys* sys,uint8_t keypress);
uint8_t incorrect_state(Sys* sys,uint8_t keypress);
uint8_t logged_in_state(Sys* sys,uint8_t keypress);
uint8_t check_user_exists(Sys *sys);


/**
 * Defines the organization of the FSM
 * for P1
 * Ex. FSM[i] = { state_function, {NEXT_STATE0, NEXT_STATE1, ...}}
 */
State FSM[7] = {
                 {init_state,{INIT_STATE_LOC,READ_USERNAME_STATE_LOC,PROCESS_NUMBER_STATE_LOC,PROCESS_ASCII_STATE_LOC,READ_PASSWORD_STATE_LOC,INCORRECT_STATE_LOC,LOGGED_IN_STATE_LOC}},
                 {read_username_state,{INIT_STATE_LOC,READ_USERNAME_STATE_LOC,PROCESS_NUMBER_STATE_LOC,PROCESS_ASCII_STATE_LOC,READ_PASSWORD_STATE_LOC,INCORRECT_STATE_LOC,LOGGED_IN_STATE_LOC}},
                 {process_number_state,{INIT_STATE_LOC,READ_USERNAME_STATE_LOC,PROCESS_NUMBER_STATE_LOC,PROCESS_ASCII_STATE_LOC,READ_PASSWORD_STATE_LOC,INCORRECT_STATE_LOC,LOGGED_IN_STATE_LOC}},
                 {process_ascii_state,{INIT_STATE_LOC,READ_USERNAME_STATE_LOC,PROCESS_NUMBER_STATE_LOC,PROCESS_ASCII_STATE_LOC,READ_PASSWORD_STATE_LOC,INCORRECT_STATE_LOC,LOGGED_IN_STATE_LOC}},
                 {read_password_state,{INIT_STATE_LOC,READ_USERNAME_STATE_LOC,PROCESS_NUMBER_STATE_LOC,PROCESS_ASCII_STATE_LOC,READ_PASSWORD_STATE_LOC,INCORRECT_STATE_LOC,LOGGED_IN_STATE_LOC}},
                 {incorrect_state,{INIT_STATE_LOC,READ_USERNAME_STATE_LOC,PROCESS_NUMBER_STATE_LOC,PROCESS_ASCII_STATE_LOC,READ_PASSWORD_STATE_LOC,INCORRECT_STATE_LOC,LOGGED_IN_STATE_LOC}},
                 {logged_in_state,{INIT_STATE_LOC,READ_USERNAME_STATE_LOC,PROCESS_NUMBER_STATE_LOC,PROCESS_ASCII_STATE_LOC,READ_PASSWORD_STATE_LOC,INCORRECT_STATE_LOC,LOGGED_IN_STATE_LOC}}
};

/**
 * Prompts the user for a username
 * and proceeds unconditionally to
 * the READ_USERNAME_STATE
 *
 * Initializes default system values
 */
uint8_t init_state(Sys* sys,uint8_t keypress)
{
    set_address_counter_4(0,0); // Write to the top left of the screen
    clear_display_4(); // Clear any old data from LCD
    write_string_4("EnterUser: "); // Write to LCD

    sys->current_user_index=0; // Initialize default system values
    sys->letter_counter=0;
    sys->number_counter=0;

    return READ_USERNAME_STATE; // Go to READ_USERNAME_STATE
}

/**
 * Computes the equivalent value of base^exp
 *
 * Original source found here: https://stackoverflow.com/a/25525853
 */
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

/**
 * Compares current username entry to values stored
 * in the known usernames array to verify username's
 * existence.
 */
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
                sys->current_user_index = i; // Where to find current username in username array
                return READ_PASSWORD_STATE; // Username match go to password state
            }
        }
    }
    return INCORRECT_STATE; // If there is no username match go to incorrect username state
}

/**
 * Functionality for handling username input
 */
uint8_t read_username_state(Sys *sys, uint8_t key_press)
{
    uint8_t curr_press = 0; // Keeps track of which key has been pressed

    if(sys->letter_counter == 4) // Check to see if username exists if we get to the last letter
    {
        sys->prev_state = READ_USERNAME_STATE; // To remember where you came from in next state
        sys->letter_counter = 0; // Reset letter counter to build future usernames
        return check_user_exists(sys);  // Goes to password if user exits, otherwise to incorrect state
    }

    curr_press=update_key_press(curr_press); // Polls until new user input to keypad
    sys->curr_input = curr_press; // Store the current input from the keypad

    sys->prev_state = READ_USERNAME_STATE; // To remember where you came from in next state


    if(sys->curr_input != KEY_POUND && sys->curr_input != KEY_STAR && sys->curr_input != 0) // If key press is a number
    {

        return PROCESS_NUMBER_STATE; // Go to process number state
    }

    return READ_USERNAME_STATE; // If  other button is pressed default back to same state

}



/**
 * Process number state:
 *  adds the number that was inputted to the second line
 *  of the display
 */
uint8_t process_number_state(Sys *sys, uint8_t key_press)
{

    set_address_counter_4(1, sys->number_counter); // Sets address counter to write newest number to LCD

    write_key_to_LCD(sys->curr_input); // Write newest number to LCD

    if (sys->prev_state == READ_USERNAME_STATE) {

        // Build up ASCII code for character
        uint8_t number_pressed = get_number_pressed(sys->curr_input);
        //manages the shift of the numbers
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

/**
 * Process ascii state:
 *  processes the numbers that have already been
 *  inputted through username/process number state
 *
 */
uint8_t process_ascii_state(Sys *sys, uint8_t key_press)
{
    sys->number_counter = 0; // reset number counter for next ascii letter

    set_address_counter_4(0, sys->letter_counter+10); // write letter passed prompt
    write_char_4(sys->curr_user[sys->letter_counter]); // Write current letter of username to LCD
    ++(sys->letter_counter); // Letter has been added to screen

    set_address_counter_4(1, 0);//move to the new line
    write_string_4("    ");//makes room to align text
    set_address_counter_4(1, 0);//move to the new line
    sys->prev_state = PROCESS_ASCII_STATE; // To remember where you came from

    return READ_USERNAME_STATE; // Go here when letter has been written to screen
}


/**
 * compares passwords of length 5 at one and two
 *  return: 1 if same, 0 if different
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
 * Read password state:
 *  reads in the pressed keypad button and stores it in the curr_pin
 *  once the curr_pin is full, then compare to the already stored password
 *  if wrong, then go to the incorrect state
 *  else, logged in state
 */
uint8_t read_password_state(Sys* sys,uint8_t keypress)
{

    uint8_t curr_press = 0;
    if(sys->prev_state==READ_USERNAME_STATE||
            sys->prev_state==INCORRECT_STATE)//if we are not already in the password state previously
    {
        clear_display_4();
        set_address_counter_4(0,0);
        write_string_4("password: ");//writes "password:" to the top row of the LCD
        sys->prev_state=READ_PASSWORD_STATE;
    }
    curr_press=update_key_press(curr_press);//poll for a keypress
    sys->curr_input = curr_press;
    sys->curr_pin[sys->number_counter] =
            get_number_pressed(sys->curr_input);//store the pressed number into the current pin
    sys->prev_state=READ_PASSWORD_STATE;//sets the previous state

    if(sys->number_counter<4)//if we don't have all the numbers, then get some more
    {
        return PROCESS_NUMBER_STATE;//go to the process number state
    }

    if(sys->number_counter==4)//once we have all 5 numbers, check for validity
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
            //incorrect auth
            return INCORRECT_STATE;
        }
    }
    return PROCESS_NUMBER_STATE;

}

/**
 * Incorrect state:
 *  depends on which state we came from to do an action
 *  displays the error message
 *  goes back to a respective function to re-enter input
 */
uint8_t incorrect_state(Sys* sys,uint8_t keypress)
{
    uint8_t i;
    clear_display_4();//make sure the display is clear so we can write to it
    write_string_4("get out that's wrong");//display our error message
    delay_us(3000000);//delays 3 seconds
    clear_display_4();//clears the display from the error message

    for (i = 0; i < 4; i++)
    { // intialize current user array values to 0
        sys->curr_user[i] = 0;
    }

    /*
     * if we came from the username state,
     * then we go to the init state
     */
    if(sys->prev_state==READ_USERNAME_STATE)
    {
        sys->prev_state=INCORRECT_STATE;
        return INIT_STATE;
    }
    /**
     * if we came from the password state,
     * then we go to back to the read password state
     */
    if(sys->prev_state==READ_PASSWORD_STATE)
    {
        sys->prev_state=INCORRECT_STATE;
        return READ_PASSWORD_STATE;//go back to the password state if it was wrong
    }
    //default case - should not be reached, but if so we go to init
    sys->prev_state=INCORRECT_STATE;
    return INIT_STATE;
}

/**
 * Logged in state:
 *  if we are not in the logged in state previously,
 *  then print our login message
 *  if the keypress is star, leave to init
 */
uint8_t logged_in_state(Sys* sys,uint8_t keypress)
{
    if(sys->prev_state !=LOGGED_IN_STATE)
    {
        clear_display_4();
        write_string_4("HlloTher");
        set_address_counter_4(1,0);//newline
        write_string_4("GnrlKenobi");
    }
    sys->prev_state=LOGGED_IN_STATE;
    if(keypress==KEY_STAR) //exit on star
    {
        return INIT_STATE;
    }
    else
    {
        return LOGGED_IN_STATE;
    }
}

/**
 * sets up the sys
 *  init display,keypad
 *  init sudo user
 */
void setup(Sys * sys)
{
    uint8_t i;
    uint8_t temp[5] = {8,0,0,8,5};//initial "sudo user" password
    setup_lcd_4();//start the lcd
    setup_keypad();//start the keypad
    clear_display_4();//clear whatever was on the keypad
    memcpy(sys->users[0].username,"sudo",sizeof(char)*4);//copy over the username
    memcpy(sys->users[0].password,temp,sizeof(uint8_t)*5);//copy over the password

    for (i = 0; i < 4; i++)
    { // intialize current user array values to 0
        sys->curr_user[i] = 0;
    }

    sys->user_count=1;//now there's only one user
    sys->current_user_index=0;
    sys->letter_counter=0;
    sys->number_counter=0;
}

/**
 * main of project 1
 *  initializes with setup
 *  starts the FSM at init
 *  runs FSM
 */
void run_P1(void)
{
    State * curr_state;
    uint8_t state_input;
    uint8_t keypress=0; //we have no keypress
    Sys sys;
    setup(&sys); //sets up the sys struct
    curr_state = INIT_STATE_LOC;//enter username
    state_input = (curr_state->state_function)(&sys,keypress);
    curr_state = curr_state->next[state_input];//set the next state
    delay_us(50000); //wait for a bit
    while(1)
    {
            keypress = get_key_pressed();    //get ASCII of the key pressed
            state_input = (curr_state->state_function)(&sys,keypress);//calls respective function to the state that we are in
            curr_state = curr_state->next[state_input];//sets the next state based on the prev state
    }
}
