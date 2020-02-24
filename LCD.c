/*
 * LCD.c
 *
 *  Created on: Feb 2, 2020
 *      Author: helen
 */

#include "msp.h"
#include "delay.h"
#include "LCD.h"

void init_LCD(void){
    // setup GPIO pins
    //P4-> SEL0 &= ~(DB4 | DB5 | DB6 | DB7);  // init p4 pins
    //P4-> SEL1 &= ~(DB4 | DB5 | DB6 | DB7);  // GPIO setup
    P4-> DIR |= (DB4 | DB5 | DB6 | DB7);    // set pins to output mode
    P4-> OUT &= ~(DB4 | DB5 | DB6 | DB7);   // clear pins

    //P3-> SEL0 &= ~(RS | RW | EN);   // init p3 pins
    //P3-> SEL1 &= ~(RS | RW | EN);   // GPIO setup
    P3-> DIR |= (RS | RW | EN);     // set pins to output mode
    P3-> OUT &= ~(RS | RW | EN);    // clear pins

    delay_us(40000, FREQ_12_MHZ);   // power up delay

    P4-> OUT |= 0x30;

    delay_us(5000, FREQ_12_MHZ);    // 5ms delay
    nybble();
    delay_us(160, FREQ_12_MHZ);     // 160us delay
    nybble();
    delay_us(160, FREQ_12_MHZ);     // 160us delay
    nybble();
    delay_us(160, FREQ_12_MHZ);     // 160us delay

    P4-> OUT |= 0x20;
    nybble();

    //function set
    command_LCD(FUNC_SET);
    command_LCD(FUNC_SET);      // 4bit, 2 line, 5x8 font
    command_LCD(0x10);          // set cursor
    command_LCD(DISPLAY_ON);    // set display, cursor, blinking
    command_LCD(CURSOR_RIGHT);  // set cursor entry mode
}

void clear_LCD(void) {
    command_LCD(CLR_DISP);
    delay_us(2000, FREQ_12_MHZ);            // delay 2ms
}

void command_LCD(unsigned char command) {
    //P4-> OUT |= command;
    P4-> OUT &= ~(DB4 | DB5 |
                  DB6 | DB7);
    P4-> OUT |= command & (DB4 | DB5 |
                           DB6 | DB7);  // put data on OUT port
    P3-> OUT &= ~RS;                    // D/I low
    P3-> OUT &= ~RW;                    // R/W = 0 --> Write

    nybble();

    //command = command << 4;
    P4-> OUT |= (command << 4);

    nybble();
}

void write_char_LCD(unsigned char letter) {

    P4-> OUT &= ~(DB4 | DB5 |
                  DB6 | DB7);           // put data on OUT port
    P4-> OUT |= letter & (DB4 | DB5 |
                          DB6 | DB7);   // put data on output port
    P3-> OUT |= RS;                     // D/I = high, send data
    P3-> OUT &= ~RW;                    // R/W = 0 --> Write

    nybble();

    //letter = letter << 4;
    P4-> OUT &= ~(DB4|DB5|DB6|DB7);
    P4-> OUT |= (letter << 4) & (DB4|DB5|DB6|DB7);

    nybble();

    //P3-> OUT &= ~RS;                        // register select low
}

/*
 * void write_string_LCD(char* string) {       // loop until printed each char of string
    while(*string) {
        write_char_LCD(*string);            // write to LCD
        string++;                           // count "up" next char in string
    }
}
*/

void nybble(void) {
    P3-> OUT |= EN;                         // enable high
    delay_us(100, FREQ_12_MHZ);             // delay 0.1ms
    P3-> OUT &= ~EN;                        // enable low
}
