/*
 * Project1FSM.h
 *
 *  Created on: Apr 26, 2020
 *      Author: Alexander
 */

#ifndef PROJECT1FSM_H_
#define PROJECT1FSM_H_

#include "keypad.h"
#include <string.h>

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

typedef struct State
{
    uint8_t (*state_function)(Sys*,uint8_t);
    struct State *next[7];
} State;


#define INIT_STATE 0
#define READ_USERNAME_STATE 1
#define PROCESS_NUMBER_STATE 2
#define PROCESS_ASCII_STATE 3
#define READ_PASSWORD_STATE 4
#define INCORRECT_STATE 5
#define LOGGED_IN_STATE 6

#define INIT_STATE_LOC &FSM[0]
#define READ_USERNAME_STATE_LOC &FSM[1]
#define PROCESS_NUMBER_STATE_LOC &FSM[2]
#define PROCESS_ASCII_STATE_LOC &FSM[3]
#define READ_PASSWORD_STATE_LOC &FSM[4]
#define INCORRECT_STATE_LOC &FSM[5]
#define LOGGED_IN_STATE_LOC &FSM[6]

uint8_t init_state(Sys* sys,uint8_t keypress);
uint8_t read_username_state(Sys* sys,uint8_t keypress);
uint8_t process_number_state(Sys* sys,uint8_t keypress);
uint8_t process_ascii_state(Sys* sys,uint8_t keypress);
uint8_t read_password_state(Sys* sys,uint8_t keypress);
uint8_t incorrect_state(Sys* sys,uint8_t keypress);
uint8_t logged_in_state(Sys* sys,uint8_t keypress);
uint8_t check_user_exists(Sys *sys);
void setup(Sys * sys);

#endif /* PROJECT1FSM_H_ */
