/*
 * LCD.h
 *
 *  Created on: Feb 2, 2020
 *      Author:
 */

#ifndef LCD_H_
#include <stdint.h>
#define LCD_H_

//  1   2  3    4   5     6    7   8   9  10   11   12   13    14   15   16
// VSS VDD V0   RS  RW   EN   DB0 DB1 DB2 DB3  DB4  DB5  DB6   DB7  LED+ LED-
// GND 3V3 GND P3.5 P3.6 P3.7                 P4.4  P4.5 P4.6  P4.7  5V  GND

#define RS BIT5         // P3.5 Register Select.
#define RW BIT6         // P3.6 Read/Write Select. RW=1: Read, RW=0: Write
#define EN BIT7         // P3.7 Operation Enable. falling edge triggered

#define DB4 BIT4        // P4.4 data bit 0
#define DB5 BIT5        // P4.5
#define DB6 BIT6        // P4.6
#define DB7 BIT7        // P4.7

#define CLR_DISP        0x01    // clear display
#define RETURN_HOME     0x02    // cursor to top left position
#define CURSOR_LEFT     0x04    // move cursor left, DDRAM decreased
#define CURSOR_RIGHT    0x06    // move cursor right, DDRAM increased
#define DISPLAY_ON      0x0F    // display on, cursor on, blinking cursor on
#define FUNC_SET        0x28    // 4 bit, 1 line, 5x8 font
#define WRITE_DATA      0x20    // write data on | D7-D0 address
#define READ_DATA       0x30    // read data from ram
#define SET_CURSOR      0x10    // set cursor
#define ENABLE_MOVE     0X80

void init_LCD(void);                        // initialize LCD
void clear_LCD(void);                       // clear display
void command_LCD(unsigned char command);    // set LCD properties
void write_char_LCD(unsigned char letter);  // write character to LCD
void write_string_LCD(char* string);        // write string to LCD
void home_LCD(void);                        // move cursor top left
void SetCursorLocation(char place);         // set DDRAM location address
void write_LCD_L1(char* string);            // write string to line 1
void write_LCD_L2(char* string);            // write string to line 2
void write_LCD_L3(char* string);            // write string to line 3
void write_LCD_L4(char* string);            // write string to line 4
void nybble(void);
void HomeScreen(void);
void A_1_MANUAL(void);
void A_2_MANUAL(void);
void B_1_ALGORITHM(void);
void B_2_ALGORITHM(void);
void C_1_DEMO(void);
void C_2_DEMO(void);
void lcd_test(void);

#endif /* LCD_H_ */
