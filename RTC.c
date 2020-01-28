/*
 * RTC.c
 *
 *  Created on: Jan 2, 2020
 *      Author: AviMaitra
 */
#include "RTC.h"
#include "msp.h"
#include "global.h"

#define HOUR_MASK 0x1F
#define MINUTE_MASK 0x3F

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
}

// RTC interrupt service routine
void RTC_C_IRQHandler(void)
{
    //Check if the fields are ready to read
    if (RTCCTL0 & RTCTEVIFG)
    {
        uint16_t currentHour;
        uint16_t currentMin;
        currentHour = RTCHOUR & HOUR_MASK;
        currentMin = RTCMIN & MINUTE_MASK;

        // Convert to decimal value used as an index
        globalCurrentTime = (currentHour*60) + currentMin;


        // TODO Have function call with current time result to get rid of global variable or keep depending on application

        // Unlock and clear interrupt event
        RTCCTL0_H = RTCKEY_H ;
        RTCCTL0_L &= ~RTCTEVIFG;
        RTCCTL0_H = 0;
    }
}



