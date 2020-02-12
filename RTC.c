#include "RTC.h"
#include "msp.h"
#include "global.h"
#include <stdint.h>

// MUST CALL THIS FUNCTION BEFORE MODIFYING YEAR, MONTH, DAY, ETC...
void initRTC(void)
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
}

// MAKE SURE TO CALL THIS FUNCTION AFTER INITIAL TIME INPUT
void startRTC(void)
{
    RTCCTL1 &= ~(RTCHOLD);                    // Start RTC calendar mode
    RTCCTL0_H = 0;                            // Lock the RTC registers
    NVIC->ISER[0] = 1 << ((RTC_C_IRQn) & 31);
}

void setRTCYear(uint16_t inputYear)
{
    RTCYEAR = inputYear;
}

uint16_t getRTCYear(void)
{
    uint16_t myYear = RTCYEAR;
    return myYear;
}

void setRTCMonth(uint8_t inputMonth)
{
    RTCMON = inputMonth;
}

uint8_t getRTCMonth(void)
{
    uint8_t myMonth = RTCMON;
    return myMonth;
}

void setRTCDay(uint8_t inputDay)
{
    RTCDAY = inputDay;
}

uint8_t getRTCDay(void)
{
    uint8_t myDay = RTCDAY;
    return myDay;
}

void setRTCHour(uint8_t inputHour)
{
    RTCHOUR = inputHour;
}

uint8_t getRTCHour(void)
{
    uint8_t myHour = RTCHOUR;
    return myHour;
}

void setRTCMinute(uint8_t inputMin)
{
    RTCMIN = inputMin;
}

uint8_t getRTCMinute(void)
{
    uint8_t myMinute = RTCMIN;
    return myMinute;
}

uint16_t getCurrentTime(void)
{
    uint16_t myTime = (getRTCHour()*60) + getRTCMinute();
    return myTime;
}

// RTC interrupt service routine
void RTC_C_IRQHandler(void)
{
    //Check if the fields are ready to read
    if (RTCCTL0 & RTCTEVIFG)
    {
        // TODO Have function callback that uses the current time to access an array of angles

        // Unlock and clear interrupt event
        RTCCTL0_H = RTCKEY_H ;
        RTCCTL0_L &= ~RTCTEVIFG;
        RTCCTL0_H = 0;
    }
}



