#include "Keypad.h"
#include "delay.h"
#include "msp.h"
#include <stdint.h>
#include "UART.h"
#include "LCD.h"

static uint8_t select = 0;

void Start_screen(void)
{
    StartScreen();

}

void Home_screen(void)
{
    HomeScreen();
}

void Select_modes(void)
{
   switch(Keypad_GetKey())
    {
        case 65:                                //Keypress = A (manual mode)
        {
            select = 65;                         //for if statements in the other cases
            A1_MANUAL();
            Manual_input();
        }
        break;

        case 66:                                //Keypress = B (algorithm mode)
        {
            select = 66;
            B1_ALGORITHM();
            Algorithm_based();

        }
        break;

        case 67:                                //Keypress = C (DEMO mode)
        {
            select = 67;
            C1_DEMO();
            Demo();
        }
        break;

        case 42:                                //Keypress = * (Back button also clear)
        {

            back();
        }
        break;

        case 35:                                //Keypress = # (enter)
        {
           if (select == 65)                   //after user enters the angle for manual input
            {
                A2_MANUAL();
                LCD_Cursor_Location(0x0D);
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

void Manual_input(void)
{
    LCD_Cursor_Location(0x0C);
    LCD_Write_Char(Keypad_GetKey);
    LCD_Write_Char(Keypad_GetKey);
}

void Algorithm_based(void)
{
    //show angle based on calculation for given time and day
    //say 'done' and '.Home'
    B2_ALGORITHM();
}

void Demo(void)
{
    //display angle from sunrise to sunset 90:-90
    //done after reaches -90
    C2_DEMO();
}

void back(void)
{
    LCD_Cursor_Location()
}

void enter(void)
{

}
