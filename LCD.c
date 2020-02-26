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

void StartScreen(void) {
    // STARTING SCREEN
    write_LCD_L1("Startup             ");            // move everything up, get rid of this
    write_LCD_L2("Enter Date:__/__/__ ");            // remember: only have 20 char spaces
    write_LCD_L3("Enter Time:__:__    ");            // military time
    write_LCD_L4("*Clear        #Enter");
    SetCursorLocation(0x4B);
    //getkeypress function
    //replace "_" with keypress
    //once key is pressed, will cursor automatically move right, or do we need to define next location?
    //needs to wait for either Clear or Enter press before changing screens
}

void HomeScreen(void) {
    // HOME SCREEN
    write_LCD_L1("A: Manual Entry     ");
    write_LCD_L2("B: Algorithm Based  ");
    write_LCD_L3("C: Demo Mode        ");
    write_LCD_L4("*Back               ");
}

void A_1_MANUAL(void) {
    // A_1: MANUAL ENTRY INPUT
    write_LCD_L1("                    ");
    write_LCD_L2("Enter Angle: ___deg ");
    write_LCD_L3("                    ");
    write_LCD_L4("*Back  !Home  #Enter");           // Back deletes input value, Home goes to Home, Enter verifies angle desired
}

void A_2_MANUAL(void) {
    // A_2: ANGLE UPDATE
    write_LCD_L1("Present Angle:___deg");
    write_LCD_L2("                    ");
    write_LCD_L3("Done.               ");           // only displayed when reach angle
    write_LCD_L4("*Back  !Home        ");
}

void B_1_ALGORITHM(void) {
    // B_1: ALGORITHM BASED
    write_LCD_L1("                    ");
    write_LCD_L2("                    ");
    write_LCD_L3("                    ");
    write_LCD_L4("*Back  !Home  #Enter");
}

void B_2_ALGORITHM(void) {
    // B_2: ANGLE UPDATE
    write_LCD_L1("Present Time:__:__  ");
    write_LCD_L2("Present Angle:___deg");
    write_LCD_L3("Done.               ");
    write_LCD_L4("*Back  !Home        ");
}

void C_1_DEMO(void) {
    // C_1: DEMO
    write_LCD_L1("Choose Demo Speed:  ");
    write_LCD_L2("A, B, C             ");
    write_LCD_L3("                    ");
    write_LCD_L4("       !Home  #Enter");
}

void C_2_DEMO(void) {
    write_LCD_L1("Percent Complete:__%");
    write_LCD_L2("Present Angle:___deg");
    write_LCD_L3("Done.               ");
    write_LCD_L4("*Back          !Home");
}

void lcd_test(void) {
    // write LCD Test
    write_LCD_L1("This now kinda worksz");           // resolved
    write_LCD_L2("This now kindb works");           // resolved
    write_LCD_L3("This now kindc works");
    write_LCD_L4("This now kindd works");
}
