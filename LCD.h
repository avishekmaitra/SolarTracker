/*
 * LCD.h
 *
 *  Created on: Feb 2, 2020
 *      Author:
 */

#ifndef LCD_H
#define LCD_H
#include <stdint.h>


// LCD Pinout
// 1   2   3   4    5    6    7    8    9    10   11   12   13   14   15   16
// VSS VDD V0  RS   RW   EN   DB0  DB1  DB2  DB3  DB4  DB5  DB6  DB7  LED+ LED-
// GND 3V3 GND P3.5 P3.6 P3.7                     P4.4 P4.5 P4.6 P4.7 5V   GND

#define RS BIT5                             // P3.5 Register Select. RS=1: Send Data, RS=0: Send Instr
#define RW BIT6                             // P3.6 Read/Write Select. RW=1: Read, RW=0: Write
#define EN BIT7                             // P3.7 Operation Enable. falling edge triggered

#define DB4 BIT4                            // P4.4
#define DB5 BIT5                            // P4.5
#define DB6 BIT6                            // P4.6
#define DB7 BIT7                            // P4.7

#define CLR_DISP        0x01                // clear display
#define RETURN_HOME     0x02                // cursor to top left position
#define CURSOR_LEFT     0x04                // move cursor left, DDRAM decreased
#define CURSOR_RIGHT    0x06                // move cursor right, DDRAM increased
#define DISPLAY_ON      0x0F                // display on, cursor on, blinking cursor on
#define FUNC_SET        0x28                // 4 bit, 2 line, 5x8 font
#define WRITE_DATA      0x20                // write data on | D7-D0 address
#define READ_DATA       0x30                // read data from ram
#define SET_CURSOR      0x10                // set cursor
#define ENABLE_MOVE     0X80

void LCD_Init(void);                        // initialize LCD
void LCD_Clear(void);                       // clear display
void nybble(void);
void LCD_Command(unsigned char command);    // set LCD properties
void LCD_Write_Char(unsigned char letter);  // write character to LCD
void LCD_Write_String(char* string);        // write string to LCD
void LCD_write(void);                       // move cursor top left
void LCD_SetCursorLocation(char place);       // set DDRAM location address
void LCD_Write_L1(char* string);            // write string to line 1
void LCD_Write_L2(char* string);            // write string to line 2
void LCD_Write_L3(char* string);            // write string to line 3
void LCD_Write_L4(char* string);            // write string to line 4
void LCD_Test(void);

void LCD_SetWelcomeScreen(void);
void LCD_SetStartScreen(void);
void LCD_SetHomeScreen(void);
void LCD_SetManualScreen(void);
void A2_MANUAL(void);
void B1_ALGORITHM(void);
void LCD_SetAlgoScreen(void);
void LCD_SetDemoScreen(void);
void C2_DEMO(void);

#endif /* LCD_H_ */
