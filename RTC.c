#include "RTC.h"
#include "msp.h"

static bool minuteEventFlag = false;
// MUST CALL THIS FUNCTION BEFORE MODIFYING YEAR, MONTH, DAY, ETC...
void RTC_Init(void)
{
    // Configure RTC
    RTCCTL0_H = RTCKEY_H ;                  // Unlock RTC key protected registers
    RTCCTL0_L |= RTCTEVIE ;                 // Enable interrupts based on RTC
    RTCCTL0_L &= ~(RTCTEVIFG);
    RTCCTL1 = RTCHOLD | RTCMODE;
    // RTC enable, Hexadecimal mode, RTC hold
    // enable RTC read ready interrupt
    // enable RTC time event interrupt every minute
    RTCSEC = 0x2D;   // Hard-code in seconds
    RTCDAY = 0x01;   // Hard-code in days
}

// MAKE SURE TO CALL THIS FUNCTION AFTER INITIAL TIME INPUT
void RTC_Start(void)
{
    RTCCTL1 &= ~(RTCHOLD);                    // Start RTC calendar mode
    RTCCTL0_H = 0;                            // Lock the RTC registers
}

void RTC_EnableInterrupt(void)
{
    NVIC->ISER[0] = 1 << ((RTC_C_IRQn) & 31);
}

void RTC_DisableInterrupt(void)
{
    NVIC->ISER[0] = 0 << ((RTC_C_IRQn) & 31);
}

void RTC_SetEventFlag(void)
{
    minuteEventFlag = true;
}

void RTC_ResetEventFlag(void)
{
    minuteEventFlag = false;
}

bool RTC_HasEventOccured(void)
{
    return minuteEventFlag;
}

void RTC_SetYear(uint16_t inputYear)
{
    RTCYEAR = inputYear;
}

uint16_t RTC_GetYear(void)
{
    uint16_t myYear = RTCYEAR;
    return myYear;
}

void RTC_SetMonth(uint8_t inputMonth)
{
    RTCMON = inputMonth;
}

uint8_t RTC_GetMonth(void)
{
    uint8_t myMonth = RTCMON;
    return myMonth;
}

void RTC_SetDay(uint8_t inputDay)
{
    RTCDAY = inputDay;
}

uint8_t RTC_GetDay(void)
{
    uint8_t myDay = RTCDAY;
    return myDay;
}

void RTC_SetHour(uint8_t inputHour)
{
    RTCHOUR = inputHour;
}

uint8_t RTC_GetHour(void)
{
    uint8_t myHour = RTCHOUR;
    return myHour;
}

void RTC_SetMinute(uint8_t inputMin)
{
    RTCMIN = inputMin;
}

uint8_t RTC_GetMinute(void)
{
    uint8_t myMinute = RTCMIN;
    return myMinute;
}

double RTC_GetCurrentTime(void)
{
    double myTime = (double)RTC_GetHour() + ((double)RTC_GetMinute() / 60.0);
    return myTime;
}

uint16_t RTC_GetCurrentDay(void)
{
    // Month starts at 1, so the -1 corrects for that
    uint16_t myDay = ((RTC_GetMonth() - 1) * 30) + RTC_GetDay();
    return myDay;
}

// RTC interrupt service routine
void RTC_C_IRQHandler(void)
{
    //Check if the fields are ready to read
    if (RTCCTL0 & RTCTEVIFG)
    {
        // Set flag
        RTC_SetEventFlag();

        // Unlock and clear interrupt event
        RTCCTL0_H = RTCKEY_H ;
        RTCCTL0_L &= ~RTCTEVIFG;
        RTCCTL0_H = 0;
    }
}
