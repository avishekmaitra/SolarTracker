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
#include "UART.h"

void main(void)
{
    // stop watchdog timer
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;

    // Set clock to 24 MHz
    set_DCO(FREQ_24_MHZ);

    __enable_irq();

	// MUST CALL THIS FUNCTION BEFORE MODIFYING YEAR, MONTH, DAY, ETC...
	RTC_Init();
	I2C_Init(ACCEL_ADDRESS);
	Keypad_Init();
	LCD_Init();
	ACCEL_Init();
	ACCEL_Calibrate();
	LCD_Clear();
    // TODO HAVE USER SET CURRENT YEAR,MONTH,DAY,TIME

	// MAKE SURE TO CALL THIS FUNCTION WHEN WE WANT TO START KEEPING TRACK OF TIME
	// RTC_Start();

    // SCB->SCR |= SCB_SCR_SLEEPONEXIT_Msk;

    while (1)
    {
       delay_ms(1000,FREQ_24_MHZ);
       LCD_Clear();
       int8_t angle = 0;
       angle = ACCEL_GetAngle();
       char myOutput[4];
       if(angle<100 && angle >=10)
       {
           myOutput[0] = ' ';
           myOutput[1] = (angle/10) + TO_CHAR;
           myOutput[2] = (angle%10) + TO_CHAR;
       }
       else if(angle<10 && angle>=0)
       {
           myOutput[0] = ' ';
           myOutput[1] = ' ';
           myOutput[2] = (angle%10) + TO_CHAR;
       }
       else if(angle<0 && angle>-10)
       {
           angle = angle*-1;
           myOutput[0] = ' ';
           myOutput[1] = '-';
           myOutput[2] = (angle%10) + TO_CHAR;
       }
       else if(angle>-100)
       {
           angle = angle*-1;
           myOutput[0] = '-';
           myOutput[1] = (angle/10) + TO_CHAR;
           myOutput[2] = (angle%10) + TO_CHAR;
       }
       myOutput[3]='\0';
       LCD_Write_L1(myOutput);
    }
}
