#include "msp.h"
#include "delay.h"
#include "UART.h"

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
	while(1)
	{
	    write_UART("Hello ");
	}
}
