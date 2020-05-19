/*
 * LCD.c
 *
 *  Created on: Feb 2, 2020
 *      Author: helen
 */

#include "msp.h"
#include "delay.h"
#include "LCD.h"
#define TO_CHAR 0x30

void LCD_Init(void)                         //setup GPIO pins
{
    P4-> SEL0 &= ~(DB4 | DB5 | DB6 | DB7);  //initialize P4 pins
    P4-> SEL1 &= ~(DB4 | DB5 | DB6 | DB7);  //GPIO setup
    P4-> DIR |=   (DB4 | DB5 | DB6 | DB7);  //set pins to output mode

    P3-> SEL0 &= ~(RS | RW | EN);           //initialize P3 pins
    P3-> SEL1 &= ~(RS | RW | EN);           //GPIO setup
    P3-> DIR |= (RS | RW | EN);             //set pins to output mode

    delay_ms(2,FREQ_24_MHZ);

    P4-> OUT = 0x00;                        //clear output pins
    P3-> OUT = 0x00;                        //clear output pins
    delay_ms(100,FREQ_24_MHZ);              //power up delay
    P4 -> OUT = 0x30;

    delay_ms(30,FREQ_24_MHZ);               //power up delay
    nybble();
    delay_ms(10,FREQ_24_MHZ);
    nybble();
    delay_ms(10,FREQ_24_MHZ);
    nybble();
    delay_ms(10,FREQ_24_MHZ);

    P4 -> OUT = 0x20;
    nybble();

    LCD_Command(FUNC_SET);                  //4 bit, 2 line, 5x8 font
    LCD_Command(SET_CURSOR);                //set cursor
    LCD_Command(DISPLAY_ON);                //set display, cursor, blinking
    LCD_Command(CURSOR_RIGHT);              //set cursor direction

    delay_ms(2,FREQ_24_MHZ);
}

void LCD_Clear(void)                        //clears all characters from screen
{
    delay_ms(2, FREQ_24_MHZ);
    LCD_Command(CLR_DISP);
    delay_ms(2, FREQ_24_MHZ);
}

void LCD_Command(unsigned char command)     //reads instruction codes
{
    delay_ms(2,FREQ_24_MHZ);

    P4-> OUT = command;                     //put instruction on OUT
    P3-> OUT &= ~RS;                        //D/I = low --> Send Instruction
    P3-> OUT &= ~RW;                        //R/W = low --> Write
    nybble();

    command = command << 4;                 //analyze next 4 bits
    P4-> OUT = command;                     //shifted instruction code on OUT
    nybble();

    delay_ms(2,FREQ_24_MHZ);
}

void LCD_Write_Char(unsigned char letter)
{
    delay_ms(2,FREQ_24_MHZ);

    P4 -> OUT = letter;                     //put letter data on OUT
    P3-> OUT |= RS;                         //D/I = high --> Send Data
    P3-> OUT &= ~RW;                        //R/W = low --> Write
    nybble();

    letter = letter << 4;                   //analyze next 4 bits
    P4 -> OUT = letter;                     //shifted letter data on OUT
    nybble();

    delay_ms(2,FREQ_24_MHZ);
}

void LCD_Write_String(char* string)
{
    while(*string)                          //loop until string completed
    {
        LCD_Write_Char(*string);            //write to LCD
        string++;                           //count "up" to next char in string
    }
}

void LCD_SetCursorLocation(char place)        //set cursor to DDRAM location as defined
{                                           //by the STU data sheet for 2-line mode
    char myCommand = ENABLE_MOVE + place;
    LCD_Command(myCommand);
}

void LCD_Write_L1(char* string)
{
    LCD_SetCursorLocation(0x00);              //set cursor location to DDRAM address 0x00, line 1
    LCD_Write_String(string);               //string to be assigned to line 1
}

void LCD_Write_L2(char* string)
{
    LCD_SetCursorLocation(0x40);              //set cursor location to DDRAM address 0x40, line 2
    LCD_Write_String(string);               //string to be assigned to line 2
}

void LCD_Write_L3(char* string)
{
    LCD_SetCursorLocation(0x14);              //set cursor location to DDRAM address 0x14, line 3
    LCD_Write_String(string);               //string to be assigned to line 3
}

void LCD_Write_L4(char* string)
{
    LCD_SetCursorLocation(0x54);              //set cursor location to DDRAM address 0x54, line 4
    LCD_Write_String(string);               //string to be assigned to line 4
}

void nybble(void)
{
    P3-> OUT |= EN;                         //enable high
    delay_ms(1, FREQ_24_MHZ);               //delay 1ms
    P3-> OUT &= ~EN;                        //enable low
}

void LCD_SetWelcomeScreen(void)
{
    LCD_Write_L1("Welcome!            ");
    LCD_Write_L2("                    ");
    LCD_Write_L3("Generating angles...");   //only displayed when reach desired angle
    LCD_Write_L4("                    ");
}

void LCD_SetStartScreen(void)                      //STARTING SCREEN
{
    LCD_Write_L1("Startup             ");
    LCD_Write_L2("Enter Date: __/__/__");   //remember: only have 20 char spaces
    LCD_Write_L3("Enter Time:    __:__");   //military time
    LCD_Write_L4("*Clear        #Enter");
    LCD_SetCursorLocation(0x4C);              //set cursor to first input blank space
    //getkeypress function
    //replace "_" with keypress
    //once key is pressed, will cursor automatically move right, or do we need to define next location?
    //needs to wait for either Clear or Enter press before changing screens
}

void LCD_SetHomeScreen(void)                       //HOME MODE CHOICE SCREEN
{
    LCD_Write_L1("A: Manual Entry     ");   //A, B, or C key press will decide next LCD screen
    LCD_Write_L2("B: Algorithm Based  ");
    LCD_Write_L3("C: Demo Mode        ");
    LCD_Write_L4("                    ");
    LCD_SetCursorLocation(0x54);            //set cursor to first place on L4
}

void LCD_SetManualScreen(void)                        //A_1: MANUAL ENTRY INPUT SCREEN
{
    LCD_Write_L1("Enter Angle:  ___deg");
    LCD_Write_L2("                    ");
    LCD_Write_L3("Current Angle:___deg");
    LCD_Write_L4(".Home         #Enter");   //Back deletes input value, Home goes Home, Enter verifies angle desired
    LCD_SetCursorLocation(0x0D);            //set cursor to first blank on L1
}

void LCD_SetAlgoScreen(void)                //B_2: ALGORITHM ANGLE UPDATE SCREEN
{
    LCD_Write_L1("Present Time:       ");
    LCD_Write_L2("Calc. Angle:     deg");
    LCD_Write_L3("Actual Angle:    deg");   //only displayed when reach desired angle
    LCD_Write_L4(".Home               ");
    LCD_SetCursorLocation(0x0D);            //set cursor to first location on L1
}

void LCD_SetDemoScreen(void)                 //C_1: DEMO choice screen
{
    LCD_Write_L1("Running Demo...     ");
    LCD_Write_L2("                    ");
    LCD_Write_L3("Angle:              ");
    LCD_Write_L4(".Home         #Enter");
    LCD_SetCursorLocation(0x1B);            //set cursor to blank space after angle on L3
}
