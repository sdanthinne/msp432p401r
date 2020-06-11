#include "lcd.h"
/*
 * lcd.c
 *
 *  Created on: Apr 14, 2020
 *      Author: sdanthinne,crapp,aknapen
 *  Note:
 *      Every function here has a 4-bit equivalent listed after itself.
 *      4-bit equivalents sometimes depend on 8-bit functions.
 */

/**
 * Initializes the LCD pins for output
 */
void setup_lcd_pins()
{
    P4->DIR=0xFF;//sets all of the P4 pins to OUTPUT
    P4->SEL0=0;
    P4->SEL1=0;//both of these set the pins to GPIO

    P3->SEL1 &= ~(BIT5|BIT6|BIT7);
    P3->SEL0 &= ~(BIT5|BIT6|BIT7); // Configures P3.(5,6,7) for general purpose i/o
    P3->DIR |= (BIT5|BIT6|BIT7); // Configures P3.(5,6,7) for output
}

/**
 * Initializes the LCD pins for output
 * 4 bit mode
 */
void setup_lcd_pins_4()
{
    P4->DIR|=0xF0;//sets 4 msb of the P4 pins to OUTPUT
    P4->SEL0&=~0xF0;
    P4->SEL1&=~0xF0;//both of these set the 4 msb pins to GPIO
    P3->SEL1 &= ~(BIT5|BIT6|BIT7);
    P3->SEL0 &= ~(BIT5|BIT6|BIT7); // Configures P3.(5,6,7) for general purpose i/o
    P3->DIR |= (BIT5|BIT6|BIT7); // Configures P3.(5,6,7) for output
}

/**
 * writes in instruction mode.
 * Does not overwrite target,
 * just ors
 */
void write_ins(uint8_t ins)
{
    P4->OUT |= ins;
    P3->OUT &= ~BIT5;//PUT RS IN INSTRUCTION MODE
    P3->OUT &= ~BIT6;//SELECT WRITE MODE
    delay_us(1100);
    P3->OUT|=BIT7;//enable
    delay_us(1100);
    P3->OUT^=BIT7;//disable
    delay_us(1100);
    P4->OUT&=~(ins);//clears P4: eliminates persist issues
}

/**
 * 4-bit version of write_ins
 */
void write_ins_4(uint8_t ins)
{
    write_ins(ins&(BIT7|BIT6|BIT5|BIT4));//write higher 4 bits
    write_ins(ins<<4);//write lower 4 bits
}

/*
 * writes in data mode.
 * Does not overwrite target,
 * just ors
 */
void write_data(uint8_t ins)
{
    P4->OUT |= ins;
    P3->OUT|=BIT5;//PUT RS IN DATA MODE
    P3->OUT&=~BIT6;//SELECT WRITE MODE
    delay_us(1100);
    P3->OUT|=BIT7;//enable
    delay_us(1100);
    P3->OUT^=BIT7;//disable
    delay_us(1100);
    P4->OUT&=~(ins);//clears P4: eliminates persist issues
}

/**
 * 4-bit version of write_data
 */
void write_data_4(uint8_t ins)
{
    write_data(ins&(BIT7|BIT6|BIT5|BIT4));//writes only the 4 MSB
    write_data(ins<<4);//writes the 4 LSB
}

/**
 * wakes up the LCD if off
 * uses Function set
 *
 */
void wakeup_lcd()
{
    //P3->OUT &= ~BIT7; // sets E bit low
    delay_us(100000); // delay 100 ms after powering on
    write_ins(0x30);//wake up signal Function set with 8 bit bus
    delay_us(10000);
    write_ins(0x30);
    delay_us(1000);
    write_ins(0x30);
    delay_us(1000);
}
void wakeup_lcd_4()
{
    wakeup_lcd();
    write_ins(0x20);//sets to 4-bit mode
    delay_us(1000);
}

/**
 * sets up the LCD with the following settings:
 * (all settings are in MSB->LSB order listed, proceeding the most
 * significant 1)
 * Probably remove as these could be considered magic numbers
 */
void setup_lcd()
{
    setup_lcd_pins();
    /*
     * wake LCD to prepare data
     */
    wakeup_lcd();
    /*
     * Function set:
     *  DL (data length) = 8 bit
     *  N (number of lines) = dual line
     *  F(font) = 5x8 dots
     */
    write_ins(0x38);

    /*
     * Display/Cursor:
     *  S/C (shift display/move cursor) = move cursor
     *  R/L (shift R/L) = left
     */
    write_ins(0x10);

    /*
     * Display on/off
     *  display=on
     *  cursor=on
     *  blink=off
     */
    write_ins(0x0e);

    /*
     * Entry Mode Set:
     *  I/D (Increment/Decrement) = increment
     *  s = display shift = off
     */
    write_ins(0x06);//entry mode set increment,display shift off
}
void setup_lcd_4()
{
    setup_lcd_pins_4();
    /*
     * wake LCD to prepare data
     */
    wakeup_lcd_4();
    /*
     * Function set:
     *  DL (data length) = 4 bit
     *  N (number of lines) = dual line
     *  F(font) = 5x8 dots
     */
    write_ins_4(0x28);

    /*
     * Display/Cursor:
     *  S/C (shift display/move cursor) = move cursor
     *  R/L (shift R/L) = left
     */
    write_ins_4(0x10);

    /*
     * Display on/off
     *  display=on
     *  cursor=on
     *  blink=off
     */
    write_ins_4(0x0e);

    /*
     * Entry Mode Set:
     *  I/D (Increment/Decrement) = increment
     *  s = display shift = off
     */
    write_ins_4(0x06);//entry mode set increment,display shift off
}


/**
 * clears the display
 */
void clear_display()
{
    P4->OUT=0;
    write_ins(0x1);//clear display
}
void clear_display_4()
{
    P4->OUT&=~(0xF0);
    write_ins_4(0x1);//clear display
    home_lcd_4();
}

/**
 * Homes the LCD.
 */
void home_lcd()
{
    write_ins(0x2);//home cursor
}
void home_lcd_4()
{
    write_ins_4(0x2);//home cursor
}

void write_char(uint8_t c)
{
    write_data(c);
}

void write_char_4(uint8_t c)
{
    write_data_4(c);
}

/**
 * prints a string to the display -
 * commented code is the old newline automation
 */
void write_string(uint8_t * c)
{
    //uint8_t count = 0;
    while(*c!=0)
    {
//        if(count==20)
//        {
//            //moves the address pointer to the new line
//            write_ins(0xC0);
//        }
        write_char(*c);
        //count += 1;
        c+=sizeof(uint8_t);
    }
}

/**
 * moves the cursor of the LCD to the newline
 */
void write_newline_4()
{
    write_ins_4(0xC0);
}

/**
 * overwrites the current location of the ddram counter
 * to be able to increment stuff
 */
void set_address_counter_4(uint8_t row, uint8_t col)
{
    uint8_t location=BIT7|(row*0X40)+col;
    write_ins_4(location);
}

/**
 * needs to be updated
 */
void write_string_4(uint8_t * c)
{
    //static uint8_t count=0;
    while(*c!=0)
    {
//        if(count==20)//if there are 17 chars written
//        {
//            //moves the ddram address pointer to the new line
//            write_ins_4(0xC0);
//            count=0;
//        }
        write_char_4(*c);
        //count+=1;
        c+=sizeof(uint8_t);
    }
}
