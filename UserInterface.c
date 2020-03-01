/*
#include "Keypad.h"
#include "delay.h"
#include "msp.h"
#include <stdint.h>
#include "UART.h"
#include "LCD.h"

static uint8_t select = 0;

void Start_Screen(void)
{
    StartScreen();
}

void Home_Screen(void)
{
    HomeScreen();
}

void Select_Modes(void)
{
   switch(Keypad_GetKey())
    {
        case 65:                                //Keypress = A (manual mode)
        {
            Manual_Input();
        }
        break;

        case 66:                                //Keypress = B (algorithm mode)
        {
            Algorithm_Based();
        }
        break;

        case 67:                                //Keypress = C (DEMO mode)
        {
            Demo();
        }
        break;

        case 42:                                //Keypress = * (Back button also clear)
        {
            if (select == 65)
            {
                LCD_Cursor_Location(0x0D);
                LCD_Write_String("  ");
            }
            if (select == 650)
            {
                Manual_Input();
            }

        }
        break;

        case 35:                                //Keypress = # (enter)
        {
           if (select == 65)                   //after user enters the angle for manual input
            {
                select = 650;                  //case 65_0
                A2_MANUAL();
                LCD_Cursor_Location(0x0D);
                //LCD_Write_String(angle from accelerometer)
            }
           if (select == 66)
           {
               B2_ALGORITHM();
               LCD_Cursor_Location(0x0D);
               //LCD_Write_String(angle from accelerometer)
               LCD_Cursor_Location(0x4E);
               //LCD_Write_String(angle from accelerometer)
           }
        }
        break;

        case 46:                               //Keypress = . (Home button)
        {
            Home_screen();
        }
        break;

        default:
        {
            break;
        }
}

void Manual_Input(void)
{
    select = 65;                         //for if statements in the other cases
    A1_MANUAL();
    LCD_Cursor_Location(0x0C);
    LCD_Write_Char(Keypad_GetKey);
    LCD_Write_Char(Keypad_GetKey);
}

void Algorithm_Based(void)
{
    //show angle based on calculation for given time and day
    //say 'done' and '.Home'
    select = 66;
    B1_ALGORITHM();
}

void Demo(void)
{
    //display angle from sunrise to sunset 90:-90
    //done after reaches -90
    select = 67;
    C2_DEMO();
}
*/
