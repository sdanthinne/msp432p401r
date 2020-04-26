#include "keypad.h"

/*
 * keypad.c
 *
 *  Created on: Apr 19, 2020
 *      Author: Aknapen,sdanthinne
 */

void setup_keypad()
{
    P5->DIR&=~(BIT4|BIT5|BIT6|BIT7);                //configure 5.4-5.7 as input
    P5->DIR|=(BIT0|BIT1|BIT2);                      //configure 5.0-5.2 as output
    P5->OUT&=~(BIT4|BIT5|BIT6|BIT7);                //configure 5.4-5.7 as pulldown
    P5->SEL0&=~(BIT0|BIT1|BIT2|BIT4|BIT5|BIT6|BIT7);//GPIO
    P5->SEL0&=~(BIT0|BIT1|BIT2|BIT4|BIT5|BIT6|BIT7);//GPIO
    P5->REN|=(BIT4|BIT5|BIT6|BIT7);                 //enable pullup/pulldown
}

/**
 * polls the keypad to see if there is any sort of press
 * returns the row that has a press(0-3)
 */
uint8_t has_press()
{
    P5->OUT|=(BIT0|BIT1|BIT2);              //sets all of the outputs to high
    return (P5->IN&(BIT4|BIT5|BIT6|BIT7));  //returns the row values or'd together(unique value for each row)
}

/**
 * Returns the ASCII value of the key pressed on the keypad
 */
uint8_t get_key_pressed()
{
    uint8_t i;
    uint8_t val;
    for (i = 0; i < 3; i++) {
        P5->OUT = (1 << i)|(P5->OUT&BIT3);                  //toggles the right ones on
        if ((P5->IN & (BIT4|  BIT5 | BIT6 | BIT7)) != 0)    //if the rows are not zero, if there is keypress
        {
            val = (P5->IN | P5->OUT) & ~BIT3;
            return val;              // returns ROW/COLUMN value of key pressed
        }
    }
    return 0;
}

uint8_t update_key_press(uint8_t prev_key)
{

    uint8_t curr_key_check= get_key_pressed();
    uint8_t curr_key_val = curr_key_check;
    uint8_t counter;
    uint8_t true_button = 0, false_button = 0;

    while(!true_button)
    {
        curr_key_check = get_key_pressed();
        if( (curr_key_check == curr_key_val) && (curr_key_check != 0))
        {
            counter++;
        }
        else
        {
            counter = 0;
        }
        curr_key_val = curr_key_check;

        if(counter > 150)
        {
            true_button = curr_key_val;
        }
    }

    counter = 0;
    while(!false_button)
        {
            curr_key_check = get_key_pressed();
            if( curr_key_check == 0)
            {
                counter++;
            }
            else
            {
                counter = 0;
            }

            if(counter > 150)
            {
                false_button = 1;
            }
        }

    return true_button;
}


/**
 * Finds the key and maps it to the LCD write
 * dictionary-like
 */
void write_key_to_LCD(uint8_t key)
{
    switch (key) {
        case KEY_ZERO:
            write_char_4(ZERO);
            break;
        case KEY_ONE:
            write_char_4(ONE);
            break;
        case KEY_TWO:
            write_char_4(TWO);
            break;
        case KEY_THREE:
            write_char_4(THREE);
            break;
        case KEY_FOUR:
            write_char_4(FOUR);
            break;
        case KEY_FIVE:
            write_char_4(FIVE);
            break;
        case KEY_SIX:
            write_char_4(SIX);
            break;
        case KEY_SEVEN:
            write_char_4(SEVEN);
            break;
        case KEY_EIGHT:
            write_char_4(EIGHT);
            break;
        case KEY_NINE:
            write_char_4(NINE);
            break;
        case KEY_STAR:
            write_char_4(STAR);
            break;
        case KEY_POUND:
            write_char_4(POUND);
            break;
        default:
            break;
    }
}


uint8_t get_number_pressed(uint8_t key)
{
    switch (key) {
        case KEY_ZERO:
            return 0;
        case KEY_ONE:
            return 1;
        case KEY_TWO:
            return 2;
        case KEY_THREE:
            return 3;
        case KEY_FOUR:
            return 4;
        case KEY_FIVE:
            return 5;
        case KEY_SIX:
            return 6;
        case KEY_SEVEN:
            return 7;
        case KEY_EIGHT:
            return 8;
        case KEY_NINE:
            return 9;
        case KEY_STAR:
            return 0x2A;
        default:
            return 0x23;
    }
}
