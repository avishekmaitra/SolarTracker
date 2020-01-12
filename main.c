#include "msp.h"
#include "delay.h"
#include "UART.h"
#include "RTC.h"

#define CURRENT_YEAR 0x07E4
#define HOUR_MASK 0x1F
#define MINUTE_MASK 0x3F

/**
 * main.c
 */

volatile uint16_t globalCurrentTime = 0;

void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

	// Set clock to 24 MHz
	set_DCO(FREQ_24_MHZ);

	// Start UART Communication
	initUART();
	initRTC();

	// These will be replaced by user input
	RTCYEAR = CURRENT_YEAR;                   // Year = 0x2020
    RTCMON = 0x01;                            // Month = 0x01 = January
    RTCDAY = 0x0B;                            // Day = 0x02 = 2nd
    RTCDOW = 0x04;                            // Day of week = 0x04 = Thursday
    RTCHOUR = 0x11;                           // Hour
    RTCMIN = 0x0D;                            // Minute
    RTCSEC = 0x2D;                            // Seconds

    RTCCTL1 &= ~(RTCHOLD);                    // Start RTC calendar mode

    RTCCTL0_H = 0;                            // Lock the RTC registers

    __enable_interrupt();
    NVIC->ISER[0] = 1 << ((RTC_C_IRQn) & 31);


    SCB->SCR |= SCB_SCR_SLEEPONEXIT_Msk;

    while (1)
    {
        __sleep();

    }

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

