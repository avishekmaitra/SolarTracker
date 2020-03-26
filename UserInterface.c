#include "ACCEL.h"
#include "Keypad.h"
#include "delay.h"
#include "msp.h"
#include <stdint.h>
#include <stdbool.h>
#include "UART.h"
#include "LCD.h"
#include "UserInterface.h"
#include "Relay.h"
#include "RTC.h"

#define CHAR_TO_NUM     0x30
#define START_DEMO      35
#define SET_HOME        46
#define SET_MANUAL      65
#define SET_ALGO        66
#define SET_DEMO        67
#define DEMO_GOAL_POS   60.0
#define DEMO_GOAL_NEG   -60.0

// Variables for all modes
static int8_t goalAngle;
static mode_t currentMode;

// Variables for Demo mode
static bool finishFlag = false;

void UI_SetMode(mode_t inputMode)
{
    currentMode = inputMode;
}

mode_t UI_GetMode(void)
{
    return currentMode;
}

void UI_SetGoalAngle(double newAngle)
{
    goalAngle = newAngle;
}

double UI_GetGoalAngle(void)
{
    return goalAngle;
}

void UI_RunHomeMode(void)
{
    currentMode = HOME;

    if(Keypad_GetKey() == SET_MANUAL)
    {
        LCD_Clear();
        currentMode = MANUAL;
        LCD_SetManualScreen();
        Keypad_ResetKey();
        return;
    }

    else if(Keypad_GetKey() == SET_ALGO)
    {
        LCD_Clear();
        currentMode = ALGO;
        LCD_SetAlgoScreen();
        RTC_EnableInterrupt();
        Keypad_ResetKey();
        return;
    }

    else if(Keypad_GetKey() == SET_DEMO)
    {
        LCD_Clear();
        currentMode = DEMO;
        LCD_SetDemoScreen();
        UI_SetGoalAngle(DEMO_GOAL_POS);
        finishFlag = false;
        Keypad_ResetKey();
        return;
    }
    else
    {
        Keypad_ResetKey();
        return;
    }

}

void UI_RunManualMode(void)
{
    currentMode = MANUAL;
}

void UI_RunAlgoMode(void)
{
    currentMode = ALGO;

    if(RTC_HasEventOccured())
    {
        RTC_ResetEventFlag();

    }

    if(Keypad_GetKey() == SET_HOME)
    {
        RTC_DisableInterrupt();
        LCD_Clear();
        Relay_Off();
        currentMode = HOME;
        LCD_SetHomeScreen();
        Keypad_ResetKey();
        return;
    }
    else
    {
        Keypad_ResetKey();
        return;
    }
}

void UI_RunDemoMode(void)
{
    currentMode = DEMO;
    LCD_Write_L3(ACCEL_GetAngle_String());
    if(Keypad_GetKey() == RESETKEY)
    {
        if(!finishFlag)
        {
            if(UI_GetGoalAngle() == DEMO_GOAL_POS)
            {
                if(ACCEL_GetAngle_Double() >= DEMO_GOAL_POS)
                {
                    finishFlag = true;
                    UI_SetGoalAngle(DEMO_GOAL_NEG);
                    Relay_Off();
                }
                else
                {
                    Relay_Out();
                    finishFlag = false;
                }
            }
            else if(UI_GetGoalAngle() == DEMO_GOAL_NEG)
            {
                if(ACCEL_GetAngle_Double() <= DEMO_GOAL_NEG)
                {
                    finishFlag = true;
                    UI_SetGoalAngle(DEMO_GOAL_POS);
                    Relay_Off();
                }
                else
                {
                    Relay_In();
                    finishFlag = false;
                }
            }
        }
        return;
    }
    else if(Keypad_GetKey() == START_DEMO)
    {
        finishFlag = false;
        Keypad_ResetKey();
        return;
    }
    else if(Keypad_GetKey() == SET_HOME)
    {
        LCD_Clear();
        Relay_Off();
        currentMode = HOME;
        LCD_SetHomeScreen();
        Keypad_ResetKey();
        return;
    }
    else
    {
        Keypad_ResetKey();
        return;
    }
}

void ui_evaluateKey(char manual_angle0, char manual_angle1, char manual_angle2)
{
    double setAngle;
    uint8_t countQs;
    uint8_t i;
    int8_t angleVal = '0';
    char myangle[3];
    myangle[0] = manual_angle0;                                                 //when enter is pressed put values into the arrray
    myangle[1] = manual_angle1;
    myangle[2] = manual_angle2;
    for (i = 0; i < 3; i++)
    {
        if (myangle[i] == '?')                                                          //counts question marks left after user inputs angle
        {
            countQs += 1;
        }
    }
    if (countQs == 0)
    {
        if (myangle[0] == 'A')                                                        //negative angle
        {
           uint8_t ones;
           uint8_t tens;
           tens = myangle[1] - CHAR_TO_NUM;
           ones = myangle[2] - CHAR_TO_NUM;
           angleVal = (-1)*(tens*(10) + ones);
       }
    }
    if (countQs == 1)                                                               //case for 1 ? => num,num or A,num or           error if A,A or num,A
    {
        if (myangle[0] == 'A')                                                        //negative angle -0 to -9
        {
            uint8_t ones;
            ones = myangle[1] - CHAR_TO_NUM;
            angleVal = -1*(ones);
        }
        else                                                                        //positive angle 0 to 90
        {
            uint8_t ones;
            uint8_t tens;
            ones = myangle[1] - CHAR_TO_NUM;
            tens = myangle[0] - CHAR_TO_NUM;
            angleVal = ((tens*10) + ones);
        }
    }
    if (countQs == 2)                                                                             //positive angle 0 to 9
    {
        uint8_t ones;
        ones = myangle[0] - CHAR_TO_NUM;
        angleVal = ones;
    }
    setAngle = (double)angleVal;
    UI_SetGoalAngle(setAngle);
}

void Demo_W2(void)                                                              //will change to void Manual_Input(void) after demo to dolan
{                                                            //for if statements in the other cases
    char manual_angle0;
    char manual_angle1;
    char manual_angle2;
    manual_angle0 = '?';
    manual_angle1 = '?';
    manual_angle2 = '?';
//    int8_t angleVal = '0';                                                        //# of question marks left in array (non press) uint8t

    LCD_Cursor_Location(0x0D);
    char myKey;

    //cycle waiting for first angle input
    while(Keypad_GetKey() == RESETKEY);
    myKey = Keypad_GetKey();                                            //enter first number of angle
    Keypad_ResetKey();
    manual_angle0 = myKey;
    LCD_Write_Char(manual_angle0);

    //cycle waiting for second angle input
    while(Keypad_GetKey() == RESETKEY);
    myKey = Keypad_GetKey();                                            //enter 2nd number of angle
    Keypad_ResetKey();
    if (myKey != '#')
    {
        manual_angle1 = myKey;
        LCD_Write_Char(manual_angle1);
    }
    else
    {
        ui_evaluateKey(manual_angle0, manual_angle1, manual_angle2);
        while(1)
        {
            Relay_MoveToGoal();
            delay_ms(250,FREQ_24_MHZ);
            LCD_Write_L3(ACCEL_GetAngle_String());
        }
    }

    //cycle waiting for third angle input
    while(Keypad_GetKey() == RESETKEY);
    myKey = Keypad_GetKey();
    Keypad_ResetKey();
    if (myKey != '#')
    {
        manual_angle2 = myKey;                                                          //enter 3rd number of angle
        LCD_Write_Char(manual_angle2);

    }
    else
    {
        ui_evaluateKey(manual_angle0, manual_angle1, manual_angle2);
        while(1)
        {
            Relay_MoveToGoal();
            delay_ms(250,FREQ_24_MHZ);
            LCD_Write_L3(ACCEL_GetAngle_String());
        }
    }
    while(Keypad_GetKey() == RESETKEY);
    myKey = Keypad_GetKey();
    Keypad_ResetKey();
    ui_evaluateKey(manual_angle0, manual_angle1, manual_angle2);
    while(1)
    {
        Relay_MoveToGoal();
        delay_ms(250,FREQ_24_MHZ);
        LCD_Write_L3(ACCEL_GetAngle_String());
    }
}
