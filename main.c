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

#include "ACCEL.h"
#include "delay.h"
#include "I2C.h"
#include "LCD.h"
#include "msp.h"
#include "RTC.h"
#include "Keypad.h"
#include <stdint.h>
#include "Relay.h"
#include "UART.h"
#include "UserInterface.h"

void main(void)
{
    // stop watchdog timer
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;

    // Set clock to 24 MHz
    set_DCO(FREQ_24_MHZ);

    __enable_irq();

	// System Initialization
	RTC_Init();
	I2C_Init(ACCEL_ADDRESS);
	Keypad_Init();
	LCD_Init();
	ACCEL_Init();
	Relay_Init();
	LCD_Clear();

	LCD_SetStartScreen();
	UI_EnterDateTime();

	LCD_SetWelcomeScreen();
	delay_ms(1000, FREQ_24_MHZ);

    // Prep for super loop
	LCD_SetHomeScreen();
	UI_SetMode(HOME);

    while (1)
    {
        switch (UI_GetMode())
        {
            case HOME:
                UI_RunHomeMode();
                break;
            case MANUAL:
                UI_RunManualMode();
                break;
            case ALGO:
                UI_RunAlgoMode();
                break;
            case DEMO:
                UI_RunDemoMode();
                break;
            default:
                break;
        }
    }
}
