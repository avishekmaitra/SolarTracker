#include "msp.h"
#include "delay.h"
#include "UART.h"
#include "RTC.h"

#define CURRENT_YEAR 0x07E4

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
