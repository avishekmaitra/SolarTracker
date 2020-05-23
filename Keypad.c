#include "Keypad.h"
#include "delay.h"
#include "msp.h"
#include <stdint.h>

#define TO_CHAR 0x30

static char key = RESETKEY;
static uint8_t hitFlag = 0;

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
   P2 -> OUT |= (R0|R1|R2|R3);                  //sets it high

   NVIC -> ISER[1] = 1 << ((PORT5_IRQn) & 31);
   Keypad_ResetKey();
}

void PORT5_IRQHandler(void)                     //interrupt handler on Port 5
{
   if(P5-> IFG & (C1|C2|C3|C4))
   {
       P5-> IFG &= ~(C1|C2|C3|C4);              //clear interrupt flag
       if(!hitFlag)                             //will enter if hitFlag is 0, to ensure only 1 keypress at a time
       {
           keypad_setkey();
       }
       P2 -> OUT |= (R0|R1|R2|R3);              //reset all rows high
   }
}

void keypad_setkey(void)                        //function that checks the rows and columns for which button was pressed
{
    hitFlag = 1;                                //key was pressed
    P2 -> OUT &= ~(R0|R1|R2|R3);                //set all low
    uint8_t COL = 0;

    P2 -> OUT |= R0;
    delay_ms(TIMESETKEY, CLK);
    COL = 0;
    COL = P5 -> IN & (C1|C2|C3|C4);

    if (COL != 0)                               //multiple if statements that goes through the rows to select the correct key based on column
    {
        if ((COL & C1) != 0)
        {
            key = 1 + TO_CHAR;
        }
        else if ((COL & C2) !=0)
        {
            key = 2 + TO_CHAR;
        }
        else if ((COL & C3) != 0)
        {
            key = 3 + TO_CHAR;
        }
        else
        {
            key = 65;                           //ASCII "A"
        }
        return;                                 //ensures that no other digits are pressed
    }
    P2 -> OUT &= ~(R0);                         //clears row0

    P2 -> OUT |= R1;
    delay_ms(TIMESETKEY, CLK);
    COL = 0;
    COL = P5 -> IN & (C1|C2|C3|C4);
    if (COL != 0)
    {
        if ((COL & C1) != 0)
        {
            key = 4 + TO_CHAR;
        }
        else if ((COL & C2) != 0)
        {
            key = 5 + TO_CHAR;
        }
        else if ((COL & C3) != 0)
        {
            key = 6 + TO_CHAR;
        }
        else
        {
            key = 66;                           //ASCII "B"
        }
        return;
    }
    P2 -> OUT &= ~(R1);                         //clears row1

    P2 -> OUT |= R2;
    delay_ms(TIMESETKEY, CLK);
    COL = 0;
    COL = P5 -> IN & (C1|C2|C3|C4);
    if (COL != 0)
    {
        if ((COL & C1) != 0)
        {
            key = 7 + TO_CHAR;
        }
        else if ((COL & C2) != 0)
        {
            key = 8 + TO_CHAR;
        }
        else if ((COL & C3) != 0)
        {
            key = 9 + TO_CHAR;
        }
        else
        {
            key = 67;                           //ASCII "C"
        }
        return;
    }
    P2 -> OUT &= ~(R2);                         //clears row2

    P2 -> OUT |= R3;
    delay_ms(TIMESETKEY, CLK);
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
            key = 0 + TO_CHAR;
        }
        else if ((COL & C3) != 0)
        {
            key = 35;                           //ASCII "#"
        }
        else
        {
            key = 46;                           //ASCII "."
        }
        return;
    }
    P2 -> OUT &= ~(R3);                         //clears row3
}

char Keypad_GetKey(void)                        //function that returns the key value
{
    return key;
}

void Keypad_ResetKey(void)
{
    key = RESETKEY;
    hitFlag = 0;
}
