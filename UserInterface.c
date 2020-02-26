#include "Keypad.h"
#include "delay.h"
#include "msp.h"
#include <stdint.h>
#include "UART.h"



void Start_screen(void)
{

}

void Home_screen(void)
{

}

void Select_modes(void)
{
   switch(Keypad_GetKey())
    {
        case 65:                                //Keypress = A (manual mode)
        {
            Manual_input();
        }
        break;

        case 66:                                //Keypress = B (algorithm mode)
        {
            Algorithm_based();
        }
        break;

        case 67:                                //Keypress = C (DEMO mode)
        {
            Demo();
        }
        break;

        case 42:                                //Keypress = * (Back button also clear)
        {
            back();
        }
        break;

        case 35:                                //Keypress = # (enter)
        {
            enter();
        }
        break;

        case 46:                               //Keypress = . (Home button)
        {
            Home_screen();
        }
        break;

        default:
        {
            break;
        }
}

void Manual_input(void)
{

}

void Algorithm_based(void)
{
    //show angle based on calculation for given time and day
    //say 'done' and '.Home'
}

void Demo(void)
{
    //display angle from sunrise to sunset 90:-90
    //done after reaches -90
}

void back(void)
{

}

void enter(void)
{

}
