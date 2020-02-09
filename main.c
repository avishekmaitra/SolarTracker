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

#include "delay.h"
#include "I2C.h"
#include "msp.h"
#include "RTC.h"
#include <stdint.h>
#include "UART.h"

void main(void)
{
    // stop watchdog timer
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;

	// Set clock to 24 MHz
	set_DCO(FREQ_24_MHZ);

	// MUST CALL THIS FUNCTION BEFORE MODIFYING YEAR, MONTH, DAY, ETC...
	initRTC();

	// TODO HAVE USER SET CURRENT YEAR,MONTH,DAY,TIME

	// MAKE SURE TO CALL THIS FUNCTION WHEN WE WANT TO START KEEPING TRACK OF TIME
	// startRTC();

    SCB->SCR |= SCB_SCR_SLEEPONEXIT_Msk;

    while (1)
    {
        __sleep();
    }
}
