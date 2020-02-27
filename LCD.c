/*
 * LCD.c
 *
 *  Created on: Feb 2, 2020
 *      Author: helen
 */

#include "msp.h"
#include "delay.h"
#include "LCD.h"

void init_LCD(void)                         //setup GPIO pins
    {
    P4-> SEL0 &= ~(DB4 | DB5 | DB6 | DB7);  //initialize P4 pins
    P4-> SEL1 &= ~(DB4 | DB5 | DB6 | DB7);  //GPIO setup
    P4-> DIR |=   (DB4 | DB5 | DB6 | DB7);  //set pins to output mode

    P3-> SEL0 &= ~(RS | RW | EN);           //initialize P3 pins
    P3-> SEL1 &= ~(RS | RW | EN);           //GPIO setup
    P3-> DIR |= (RS | RW | EN);             //set pins to output mode

    delay_ms(2,FREQ_48_MHZ);

    P4-> OUT = 0x00;                        //clear outputs
    P3-> OUT = 0x00;
    delay_ms(100,FREQ_48_MHZ);              //power up delay
    P4 -> OUT = 0x30;

    delay_ms(30,FREQ_48_MHZ);               //power up delay
    nybble();
    delay_ms(10,FREQ_48_MHZ);
    nybble();
    delay_ms(10,FREQ_48_MHZ);
    nybble();
    delay_ms(10,FREQ_48_MHZ);

    P4 -> OUT = 0x20;
    nybble();

    command_LCD(FUNC_SET);                  //4 bit, 2 line, 5x8 font
    command_LCD(SET_CURSOR);                //set cursor
    command_LCD(DISPLAY_ON);                //set display, cursor, blinking
    command_LCD(CURSOR_RIGHT);              //set cursor direction

    delay_ms(2,FREQ_48_MHZ);
}

void clear_LCD(void)                        //clears all characters from screen
{
    delay_ms(2, FREQ_48_MHZ);
    command_LCD(CLR_DISP);
    delay_ms(2, FREQ_48_MHZ);
}

void command_LCD(unsigned char command)     //reads instruction codes
{
    delay_ms(2,FREQ_48_MHZ);

    P4-> OUT = command;                     //put instruction on OUT
    P3-> OUT &= ~RS;                        //D/I = low --> Send Instruction
    P3-> OUT &= ~RW;                        //R/W = low --> Write
    nybble();

    command = command << 4;                 //analyze next 4 bits
    P4-> OUT = command;                     //shifted instruction code on OUT
    nybble();

    delay_ms(2,FREQ_48_MHZ);
}

void write_char_LCD(unsigned char letter)
{
    delay_ms(2,FREQ_48_MHZ);

    P4 -> OUT = letter;                     //put letter data on OUT
    P3-> OUT |= RS;                         //D/I = high --> Send Data
    P3-> OUT &= ~RW;                        //R/W = low --> Write
    nybble();

    letter = letter << 4;                   //analyze next 4 bits
    P4 -> OUT = letter;                     //shifted letter data on OUT
    nybble();

    delay_ms(2,FREQ_48_MHZ);
}

void write_string_LCD(char* string)
{
    while(*string)                          //loop until string completed
    {
        write_char_LCD(*string);            //write to LCD
        string++;                           //count "up" to next char in string
    }
}

void SetCursorLocation(char place)          //set cursor to DDRAM location as defined
{                                           //by the STU data sheet for 2-line mode
    char myCommand = ENABLE_MOVE + place;
    command_LCD(myCommand);
}

void write_LCD_L1(char* string)
{
    SetCursorLocation(0x00);                //set cursor location to DDRAM address 0x00, line 1
    write_string_LCD(string);               //string to be assigned to line 1
}

void write_LCD_L2(char* string)
{
    SetCursorLocation(0x40);                //set cursor location to DDRAM address 0x40, line 2
    write_string_LCD(string);               //string to be assigned to line 2
}

void write_LCD_L3(char* string)
{
    SetCursorLocation(0x14);                //set cursor location to DDRAM address 0x14, line 3
    write_string_LCD(string);               //string to be assigned to line 3
}

void write_LCD_L4(char* string)
{
    SetCursorLocation(0x54);                //set cursor location to DDRAM address 0x54, line 4
    write_string_LCD(string);               //string to be assigned to line 4
}

void nybble(void)
{
    P3-> OUT |= EN;                         //enable high
    delay_ms(1, FREQ_48_MHZ);               //delay 0.1ms
    P3-> OUT &= ~EN;                        //enable low
}

void StartScreen(void)                      //STARTING SCREEN
{
    write_LCD_L1("Startup             ");
    write_LCD_L2("Enter Date:__/__/__ ");   //remember: only have 20 char spaces
    write_LCD_L3("Enter Time:__:__    ");   //military time
    write_LCD_L4("*Clear        #Enter");
    SetCursorLocation(0x4B);
    //getkeypress function
    //replace "_" with keypress
    //once key is pressed, will cursor automatically move right, or do we need to define next location?
    //needs to wait for either Clear or Enter press before changing screens
}

void HomeScreen(void)                       //HOME MODE CHOICE SCREEN
{
    write_LCD_L1("A: Manual Entry     ");   //A, B, or C key press will decide next LCD screen
    write_LCD_L2("B: Algorithm Based  ");
    write_LCD_L3("C: Demo Mode        ");
    write_LCD_L4("*Back               ");
}

void A1_MANUAL(void)                        //A_1: MANUAL ENTRY INPUT SCREEN
{
    write_LCD_L1("                    ");
    write_LCD_L2("Enter Angle: ___deg ");
    write_LCD_L3("                    ");
    write_LCD_L4("*Back  !Home  #Enter");   //Back deletes input value, Home goes Home, Enter verifies angle desired
}

void A2_MANUAL(void)                        //A_2: MANUAL ENTRY ANGLE UPDATE SCREEN
{
    write_LCD_L1("Present Angle:___deg");
    write_LCD_L2("                    ");
    write_LCD_L3("Done.               ");   //only displayed when reach desired angle
    write_LCD_L4("*Back  !Home        ");
}

void B1_ALGORITHM(void)                     //B_1: ALGORITHM BASED ENTRY SCREEN
{
    write_LCD_L1("                    ");
    write_LCD_L2("                    ");
    write_LCD_L3("                    ");
    write_LCD_L4("*Back  !Home  #Enter");
}

void B2_ALGORITHM(void)                     //B_2: ALGORITHM ANGLE UPDATE SCREEN
{
    write_LCD_L1("Present Time:__:__  ");
    write_LCD_L2("Present Angle:___deg");
    write_LCD_L3("Done.               ");   //only displayed when reach desired angle
    write_LCD_L4("*Back  !Home        ");
}

void C1_DEMO(void)                          //C_1: DEMO choice screen
{
    write_LCD_L1("Choose Demo Speed:  ");
    write_LCD_L2("A, B, C             ");
    write_LCD_L3("                    ");
    write_LCD_L4("       !Home  #Enter");
}

void C2_DEMO(void)                          //C_2: DEMO UPDATE SCREEN
{
    write_LCD_L1("Percent Complete:__%");
    write_LCD_L2("Present Angle:___deg");
    write_LCD_L3("Done.               ");   //only displayed when reach desired angle
    write_LCD_L4("*Back          !Home");
}

void lcd_test(void)                         //write LCD Test
{
    write_LCD_L1("This now kinda works");
    write_LCD_L2("This now kindb works");
    write_LCD_L3("This now kindc works");
    write_LCD_L4("This now kindd works");
}
