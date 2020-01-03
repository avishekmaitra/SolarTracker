#include "msp.h"
#include "delay.h"
#include "UART.h"
#include "RTC.h"

/**
 * main.c
 */
void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

	// Set clock to 24 MHz
	set_DCO(FREQ_24_MHZ);

	// Start UART Communication
	initUART();
	initRTC();

	RTCYEAR = 0x2020;                         // Year = 0x2020
    RTCMON = 0x01;                            // Month = 0x01 = January
    RTCDAY = 0x02;                            // Day = 0x02 = 2nd
    RTCDOW = 0x04;                            // Day of week = 0x01 = Thursday
    RTCHOUR = 0x15;                           // Hour = 0x15
    RTCMIN = 0x23;                            // Minute = 0x23
    RTCSEC = 0x45;                            // Seconds = 0x45

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

    if (RTCCTL0 & RTCTEVIFG)
    {
        writeDigit(RTCMIN);
        write_UART(" ");
        RTCCTL0_H = RTCKEY_H ;
        RTCCTL0_L &= ~RTCTEVIFG;
        RTCCTL0_H = 0;
    }

}

