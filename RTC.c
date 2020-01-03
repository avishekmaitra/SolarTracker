/*
 * RTC.c
 *
 *  Created on: Jan 2, 2020
 *      Author: AviMaitra
 */
#include "RTC.h"
#include "msp.h"

void initRTC(void)
{
    // Configure RTC
    RTCCTL0_H = RTCKEY_H ;                  // Unlock RTC key protected registers
    RTCCTL0_L |= RTCTEVIE ;                 // Enable interrupts based on RTC
    RTCCTL0_L &= ~(RTCTEVIFG);
    RTCCTL1 = RTCBCD | RTCHOLD ;
    // RTC enable, BCD mode, RTC hold
    // enable RTC read ready interrupt
    // enable RTC time event interrupt every minute
}



