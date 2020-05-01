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
#define NEGATIVE_FLAG   'A'
#define NEGATIVE_SIGN   '-'
#define NO_INPUT        RESETKEY
#define ENTER           '#'
#define MILLENNIUM      2000
#define LCD_WRITE_COUNT 1000
#define LCD_MONTH_LOC   0x4D
#define LCD_DAY_LOC     0x50
#define LCD_YEAR_LOC    0x53
#define LCD_HOUR_LOC    0x24
#define LCD_MINUTE_LOC  0x27

// Variables for all modes
static int8_t goalAngle;
static mode_t currentMode;

// Variables for Demo mode
static bool finishFlag = false;

// Variables for Manual Mode
// The state also corresponds to the length of the input
enum manualState{
    ZERO_STATE,
    FIRST_STATE,
    SECOND_STATE,
    THIRD_STATE};
static uint8_t currentState = ZERO_STATE;
static int8_t currentInput = 0;
static bool negativeFlag = false;

// Helper functions
void ui_goToGoal_manual(double inputGoal)
{
    uint16_t writeCount;
    UI_SetGoalAngle(inputGoal);
    writeCount = 0;

    while(Relay_MoveToGoal())
    {
        if(writeCount == LCD_WRITE_COUNT)
        {
            writeCount = 0;
            LCD_Write_L3(ACCEL_GetAngle_String());
            Keypad_ResetKey();                                  //added this 5/1/20 to make sure keypad is getting reset
        }
        else
        {
            writeCount = writeCount + 1;
            Keypad_ResetKey();                                  //added this 5/1/20 to make sure keypad is getting reset
        }
    }
}

void ui_reset_manual(void)
{
    LCD_Clear();
    LCD_SetManualScreen();
    LCD_Write_L3(ACCEL_GetAngle_String());
    currentState = ZERO_STATE;
    negativeFlag = false;
    currentInput = 0;
}

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

void UI_EnterDateTime(void)
{
    char inputKey;
    uint8_t month;
    uint8_t day;
    uint16_t year;

    uint8_t hour;
    uint8_t minute;

    Keypad_ResetKey();                                  //make sure hitflag starts as 0

    // Get first month input
    LCD_SetCursorLocation(LCD_MONTH_LOC);
    while(Keypad_GetKey() == RESETKEY);
    inputKey = Keypad_GetKey();
    LCD_Write_Char(inputKey);
    month = (inputKey-CHAR_TO_NUM) * 10;
    Keypad_ResetKey();

    // Get second month input
    while(Keypad_GetKey() == RESETKEY);
    inputKey = Keypad_GetKey();
    LCD_Write_Char(inputKey);
    month = month + (inputKey-CHAR_TO_NUM);
    RTC_SetMonth(month);
    Keypad_ResetKey();

    // Get first day input
    LCD_SetCursorLocation(LCD_DAY_LOC);
    while(Keypad_GetKey() == RESETKEY);
    inputKey = Keypad_GetKey();
    LCD_Write_Char(inputKey);
    day = (inputKey-CHAR_TO_NUM) * 10;
    Keypad_ResetKey();

    // Get second day input
    while(Keypad_GetKey() == RESETKEY);
    inputKey = Keypad_GetKey();
    LCD_Write_Char(inputKey);
    day = day + (inputKey-CHAR_TO_NUM);
    RTC_SetDay(day);
    Keypad_ResetKey();

    // Get first year input
    LCD_SetCursorLocation(LCD_YEAR_LOC);
    while(Keypad_GetKey() == RESETKEY);
    inputKey = Keypad_GetKey();
    LCD_Write_Char(inputKey);
    year = MILLENNIUM;
    year = year + ((inputKey-CHAR_TO_NUM) * 10);
    Keypad_ResetKey();

    // Get second year input
    while(Keypad_GetKey() == RESETKEY);
    inputKey = Keypad_GetKey();
    LCD_Write_Char(inputKey);
    year = year + (inputKey-CHAR_TO_NUM);
    RTC_SetYear(year);
    Keypad_ResetKey();

    // Get first hour input -> NEEDS TO BE MILITARY TIME
    LCD_SetCursorLocation(LCD_HOUR_LOC);
    while(Keypad_GetKey() == RESETKEY);
    inputKey = Keypad_GetKey();
    LCD_Write_Char(inputKey);
    hour = (inputKey-CHAR_TO_NUM) * 10;
    Keypad_ResetKey();

    // Get second hour input -> NEEDS TO BE MILITARY TIME
    while(Keypad_GetKey() == RESETKEY);
    inputKey = Keypad_GetKey();
    LCD_Write_Char(inputKey);
    hour = hour + (inputKey-CHAR_TO_NUM);
    RTC_SetHour(hour);
    Keypad_ResetKey();

    // Get first minute input
    LCD_SetCursorLocation(LCD_MINUTE_LOC);
    while(Keypad_GetKey() == RESETKEY);
    inputKey = Keypad_GetKey();
    LCD_Write_Char(inputKey);
    minute = (inputKey-CHAR_TO_NUM) * 10;
    Keypad_ResetKey();

    // Get second day input
    while(Keypad_GetKey() == RESETKEY);
    inputKey = Keypad_GetKey();
    LCD_Write_Char(inputKey);
    minute = minute + (inputKey-CHAR_TO_NUM);
    RTC_SetMinute(minute);
    Keypad_ResetKey();

    // Start RTC with all the input information
    RTC_Start();
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
    // Input range is from -90 to 90 degrees
    currentMode = MANUAL;
    Keypad_ResetKey();                                              //just to make sure hitflag starts as 0
    if(Keypad_GetKey() == SET_HOME)
    {
        LCD_Clear();
        Relay_Off();
        currentMode = HOME;
        LCD_SetHomeScreen();
        Keypad_ResetKey();
        return;
    }
    else if(Keypad_GetKey() == NO_INPUT)
    {
        Keypad_ResetKey();
        return;
    }
    else
    {
        if(currentState == ZERO_STATE)
        {
            LCD_SetCursorLocation(0x0D);
            if(Keypad_GetKey() == NEGATIVE_FLAG)
            {
                LCD_Write_Char(NEGATIVE_SIGN);
                negativeFlag = true;
                currentInput = 0;
            }
            else
            {
                LCD_Write_Char(Keypad_GetKey());
                negativeFlag = false;
                currentInput = Keypad_GetKey()-CHAR_TO_NUM;
            }

            // Prep for next state
            currentState = FIRST_STATE;
            Keypad_ResetKey();
            return;
        }
        if(currentState == FIRST_STATE)
        {
            if(Keypad_GetKey() == ENTER)
            {
                double myGoal;

                myGoal = (double) currentInput;
                ui_goToGoal_manual(myGoal);

                // Reset variables
                ui_reset_manual();
                Keypad_ResetKey();
                return;
            }
            else
            {
                LCD_Write_Char(Keypad_GetKey());
                // Need to adjust input depending on negativity
                if(negativeFlag)
                {
                    currentInput = Keypad_GetKey()-CHAR_TO_NUM;
                }
                else
                {
                    currentInput = (currentInput*10) + (Keypad_GetKey()-CHAR_TO_NUM);
                }

                // Prep for next state
                currentState = SECOND_STATE;
                Keypad_ResetKey();
                return;
            }
        }
        if(currentState == SECOND_STATE)
        {
            if(Keypad_GetKey() == ENTER)
            {
                double myGoal;

                if(negativeFlag)
                {
                    myGoal = (double) (-1*currentInput);
                }
                else
                {
                    myGoal = (double) currentInput;
                }

                ui_goToGoal_manual(myGoal);

                // Reset variables
                ui_reset_manual();
                Keypad_ResetKey();
                return;
            }
            else
            {
                LCD_Write_Char(Keypad_GetKey());
                // If third entry, value must be negative
                currentInput = -1 * (currentInput*10) + (Keypad_GetKey()-CHAR_TO_NUM);

                // Prep for next state
                currentState = THIRD_STATE;
                Keypad_ResetKey();
                return;
            }
        }
        if (currentState == THIRD_STATE)
        {
            if(Keypad_GetKey() == ENTER)
            {
                double myGoal;

                // Current input has already been made negative
                myGoal = (double) currentInput;
                ui_goToGoal_manual(myGoal);

                // Reset variables
                ui_reset_manual();
                Keypad_ResetKey();
                return;
            }
            else
            {
                Keypad_ResetKey();
                return;
            }
        }
    }
}

void UI_RunAlgoMode(void)
{
    currentMode = ALGO;

    if(RTC_HasEventOccured())
    {
        RTC_ResetEventFlag();
        // TODO Using algorithm from Dolan, grab an angle depending on the day and time
        // UI_SetGoalAngle(Dolan_Angle)
        while(Relay_MoveToGoal());
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

    if(Keypad_GetKey() == START_DEMO)
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
        Keypad_ResetKey();
        return;
    }
}
