/**
 * Senior Project: Solar Tracker
 *
 * ENGINEERS: AVISHEK MAITRA, DELANEY BERGER, HELEN RICE
 *
 * DESCRIPTION: This project allows users to easily control and test solar panels
 *
 * INPUT: Users input to set angle and algorithm for solar panels
 * OUTPUT: Relay output that controls movement of the solar panel
 *
 * DATE: JUNE 5TH, 2020
 *
 * SPONSOR: CALIFORNIA POLYTECHNIC STATE UNVERSITY - ELECTRICAL ENGINEERING DEPARTMENT
 */

#include "msp.h"
#include "delay.h"
#include "UART.h"
#include "RTC.h"
#include <stdint.h>

void main(void)
{
    // stop watchdog timer
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;

	// Set clock to 24 MHz
	set_DCO(FREQ_24_MHZ);

	// MUST CALL THIS FUNCTION BEFORE MODIFYING YEAR, MONTH, DAY, ETC...
	initRTC();

	// TODO HAVE USER SET CURRENT YEAR,MONTH,DAY,TIME
	uint16_t testYear = 0;
	uint8_t testMonth = 0;
	uint8_t testDay = 0;
	uint8_t testHour = 0;
	uint8_t testMinute = 0;

	setRTCYear(0x07E4);

	setRTCMonth(0x01);

	setRTCDay(0x1D);

	setRTCHour(0x06);

	setRTCMinute(0x05);

	// MAKE SURE TO CALL THIS FUNCTION AFTER INITIAL TIME INPUT
	startRTC();

	testYear = getRTCYear();
	testMonth = getRTCMonth();
	testDay = getRTCDay();
	testHour = getRTCHour();
	testMinute = getRTCMinute();

    SCB->SCR |= SCB_SCR_SLEEPONEXIT_Msk;

    while (1)
    {
        __sleep();
    }
}
