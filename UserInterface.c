
#include "Keypad.h"
#include "delay.h"
#include "msp.h"
#include <stdint.h>
#include "UART.h"
#include "LCD.h"

static uint8_t select = 0;

void Start_Screen(void)
{
    StartScreen();                                                //initially start with enter time and day
}

void Home_Screen(void)
{
    HomeScreen();                                                //gives options to select the modes: A=manual, B=algorithm, C=demo
}

void Select_Modes(void)                                          //states dependent on the keypress for (A,B,C,.,#,*)
{
   switch(Keypad_GetKey())
    {
        case 65:                                                //Keypress = A (manual mode)
        {
            Manual_Input();
        }
        break;

        case 66:                                                 //Keypress = B (algorithm mode)
        {
            Algorithm_Based();
        }
        break;

        case 67:                                                //Keypress = C (DEMO mode)
        {
            Demo();
        }
        break;

        case 42:                                                //Keypress = * (Back button also clear)
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

        case 35:                                                //Keypress = # (enter)
        {
           if (select == 65)                                    //after user enters the angle for manual input, shows the angle that tracker is at
            {
                select = 650;                                   //case 65_0
                A2_MANUAL();
                LCD_Cursor_Location(0x0D);
                //LCD_Write_String(angle from accelerometer)
            }
           if (select == 66)                                    //after enter is pressed will show angle that tracker is at
           {
               B2_ALGORITHM();
               LCD_Cursor_Location(0x0D);
               //LCD_Write_String(angle from accelerometer)
               LCD_Cursor_Location(0x4E);
               //LCD_Write_String(angle from accelerometer)
           }
        }
        break;

        case 46:                                               //Keypress = . (Home button)
        {
            Home_screen();                                     //will go back to homescreen directly
        }
        break;

        default:
        {
            break;
        }
}

char Manual_Input(void)
{
    char manual_angle1 = 0;
    char manual_angle2 = 0;
    char manual_angle3 = 0;
    char manual_angle = 0;
    select = 65;                                                               //for if statements in the other cases
    A1_MANUAL();
    LCD_Cursor_Location(0x0C);                                                //move cursor to position to allow user to input angle in desired spot
    manual_angle1 = Keypad_GetKey();                                          //enter first number of angle
    manual_angle2 = Keypad_GetKey();                                         //second number of angle
    manual_angle3 = Keypad_GetKey();                                         //third number of angle
    manual_angle = manual_angle1 + manual_angle2 + manual_angle3;           //puts into variable so can use in accelerometer
    return manual_angle;
}
int8_t ManualAngleConversion(void)                                          //conversion function since Angles are going to be given from 0-180
{                                                                           //pseudo
    char MA_accel = 0;
    if (Manual_Inuput() <= 90)
    {
        MA_accel = Manual_Input();
        return MA_accel;
    }
    else
    {
        MA_accel = Manual_Input() - '180';
        return MA_accel;
    }
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

