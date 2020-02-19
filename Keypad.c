#include "Keypad.h"
#include "delay.h"
#include "msp.h"
#include <stdint.h>
#include "UART.h"

static uint8_t key = 100;

void Keypad_Init(void)
{
   P5 -> SEL1 &= ~(C1|C2|C3|C4);
   P5 -> SEL0 &= ~(C1|C2|C3|C4);
   P5 -> DIR &= ~(C1|C2|C3|C4);                 //Set Columns(input) as interrupts
   P5 -> IES &= ~(C1|C2|C3|C4);                 //interrupt on low to high transitions
   P5 -> IFG &= ~(C1|C2|C3|C4);                 //clear interrupts
   P5 -> IE |= (C1|C2|C3|C4);                   //enable interrupts on columns
   P5 -> REN |= (C1|C2|C3|C4);                  //use enable resistor
   P5 -> OUT &= ~(C1|C2|C3|C4);                 //pull down resistors


   P2 -> DIR |= (R0|R1|R2|R3);                  //sets rows as outputs
   P2 -> SEL1 &= ~(R0|R1|R2|R3);
   P2 -> SEL0 &= ~(R0|R1|R2|R3);
   P2 -> OUT |= (R0|R1|R2|R3);                  // sets it high


   NVIC -> ISER[1] = 1 << ((PORT5_IRQn) & 31);
}

void PORT5_IRQHandler(void)                     //interrupt handler on Port 5
{
   P5-> IFG &= ~(C1|C2|C3|C4);                  //clear interrupt flag
   keypad_setkey();
}

void keypad_setkey(void)                        //function that checks the rows and columns for which button was pressed
{
    P2 -> OUT &= ~(R0|R1|R2|R3);                //set all low
    uint8_t COL = 0;

    P2 -> OUT |= R0;
    delay_ms(TIME, CLK);
    COL = 0;
    COL = P5 -> IN & (C1|C2|C3|C4);
    if (COL != 0)                               //Multiple if statements that goes through the rows to select the correct key based on column
    {
        if ((COL & C1) != 0)
        {
            key = 1;
        }
        else if ((COL & C2) !=0)
        {
            key = 2;
        }
        else if ((COL & C3) != 0)
        {
            key = 3;
        }
        else
        {
            key = 65;                           //ASCII "A"
        }
        P2 -> OUT &= ~(R0);
    }

    P2 -> OUT |= R1;
    delay_ms(TIME, CLK);
    COL = 0;
    COL = P5 -> IN & (C1|C2|C3|C4);
    if (COL != 0)
    {
        if ((COL & C1) != 0)
        {
            key = 4;
        }
        else if ((COL & C2) != 0)
        {
            key = 5;
        }
        else if ((COL & C3) != 0)
        {
            key = 6;
        }
        else
        {
            key = 66;                           //ASCII "B"
        }
        P2 -> OUT &= ~(R1);
    }

    P2 -> OUT |= R2;
    delay_ms(TIME, CLK);
    COL = 0;
    COL = P5 -> IN & (C1|C2|C3|C4);
    if (COL != 0)
    {
        if ((COL & C1) != 0)
        {
            key = 7;
        }
        else if ((COL & C2) != 0)
        {
            key = 8;
        }
        else if ((COL & C3) != 0)
        {
            key = 9;
        }
        else
        {
            key = 67;                           //ASCII "C"
        }
        P2 -> OUT &= ~(R2);
}

    P2 -> OUT |= R3;
    delay_ms(TIME, CLK);
    COL = 0;
    COL = P5 -> IN & (C1|C2|C3|C4);
    if (COL != 0)
    {
        if ((COL & C1) != 0)
        {
            key = 42;                           //ASCII "*"
        }
        else if ((COL & C2) != 0)
        {
            key = 0;
        }
        else if ((COL & C3) != 0)
        {
            key = 35;                           //ASCII "#"
        }
        else
        {
            key = 46;                           //ASCII "."
        }
    P2 -> OUT &= ~(R3);
}
}
uint8_t Keypad_GetKey(void)                     //function that returns the key value
{
    uint8_t tempkey = key;
    key = RESETKEY;
    return tempkey;
}

