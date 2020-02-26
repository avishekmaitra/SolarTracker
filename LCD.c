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
    P4-> SEL0 &= ~(DB4 | DB5 | DB6 | DB7);  // init p4 pins
    P4-> SEL1 &= ~(DB4 | DB5 | DB6 | DB7);  // GPIO setup
    P4-> DIR |= (DB4 | DB5 | DB6 | DB7);    // set pins to output mode

    P3-> SEL0 &= ~(RS | RW | EN);           // init p3 pins
    P3-> SEL1 &= ~(RS | RW | EN);           // GPIO setup
    P3-> DIR |= (RS | RW | EN);             // set pins to output mode

    delay_ms(2,FREQ_48_MHZ);

    P4-> OUT = 0x00;
    P3-> OUT = 0x00;
    delay_ms(100,FREQ_48_MHZ);              // power up delay
    P4 -> OUT = 0x30;

    delay_ms(30,FREQ_48_MHZ);               // power up delay
    nybble();
    delay_ms(10,FREQ_48_MHZ);
    nybble();
    delay_ms(10,FREQ_48_MHZ);
    nybble();
    delay_ms(10,FREQ_48_MHZ);
    P4 -> OUT = 0x20;
    nybble();

    command_LCD(FUNC_SET);                  // 4bit, 2 line, 5x8 font
    command_LCD(SET_CURSOR);                // set cursor
    command_LCD(DISPLAY_ON);                // set display, cursor, blinking
    command_LCD(CURSOR_RIGHT);              // set cursor direction

    delay_ms(2,FREQ_48_MHZ);
}

void clear_LCD(void)
{
    delay_ms(2, FREQ_48_MHZ);
    command_LCD(CLR_DISP);
    delay_ms(2, FREQ_48_MHZ);               // delay 2ms
}

void command_LCD(unsigned char command)
{

    delay_ms(2,FREQ_48_MHZ);

    P4-> OUT = command;                     // put data on OUT port

    P3-> OUT &= ~RS;                        // D/I low
    P3-> OUT &= ~RW;                        // R/W = 0 --> Write

    nybble();

    command = command << 4;                 // analyze upper 4 bits
    P4-> OUT = command;

    nybble();

    delay_ms(2,FREQ_48_MHZ);
}

void write_char_LCD(unsigned char letter)
{
    delay_ms(2,FREQ_48_MHZ);
    P4 -> OUT = letter;                     // put data on output port
    P3-> OUT |= RS;                         // D/I = high, send data
    P3-> OUT &= ~RW;                        // R/W = 0 --> Write

    nybble();

    letter = letter << 4;                   // analyze upper 4 bits
    P4 -> OUT = letter;

    nybble();

    delay_ms(2,FREQ_48_MHZ);
}

void write_string_LCD(char* string)
{       // loop until printed each char of string
    while(*string)
    {
        write_char_LCD(*string);            // write to LCD
        string++;                           // count "up" next char in string
    }
}

void SetCursorLocation(char place)
{
    //Set cursor to proper location as defined by the STU data sheet
    char myCommand = ENABLE_MOVE + place;
    command_LCD(myCommand);
}

void write_LCD_L1(char* string) {
    SetCursorLocation(0x00);                // set cursor location to DDRAM address 0x00, line 1
    write_string_LCD(string);               // string to be assigned to line 1
}

void write_LCD_L2(char* string) {
    SetCursorLocation(0x40);                // set cursor location to DDRAM address 0x40, line 2
    write_string_LCD(string);               // string to be assigned to line 2
}

void write_LCD_L3(char* string) {
    SetCursorLocation(0x14);                // set cursor location to DDRAM address 0x14, line 3
    write_string_LCD(string);               // string to be assigned to line 3
}

void write_LCD_L4(char* string) {
    SetCursorLocation(0x54);                // set cursor location to DDRAM address 0x54, line 4
    write_string_LCD(string);               // string to be assigned to line 4
}

void nybble(void) {
    P3-> OUT |= EN;                         // enable high
    delay_ms(1, FREQ_48_MHZ);               // delay 0.1ms
    P3-> OUT &= ~EN;                        // enable low
}
