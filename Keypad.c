/*
 * Keypad.c
 *
 *  Created on: Feb 2, 2020
 *      Author: Delaney Berger
 */

#include "Keypad.h"
#include "delay.h"
#include "msp.h"
#include <stdint.h>
#include "UART.h"

static uint8_t key;

void KeypadInit(void)
{
   P5 -> DIR &= ~(C1|C2|C3|C4); //interrupt Columns inputs
   P5 -> IES |= (C1|C2|C3|C4);
   P5 -> IFG &= ~(C1|C2|C3|C4);
   P5 -> IE |= (C1|C2|C3|C4);
   P5 -> SEL1 &= ~(C1|C2|C3|C4);
   P5 -> SEL0 &= ~(C1|C2|C3|C4);
   P5 -> REN = (C1|C2|C3|C4); // use enable resistor
   P5 -> OUT &= ~(C1|C2|C3|C4); //  pull down resistors


   P2 -> DIR = (R0|R1|R2|R3); //rows outputs
   /*
   P2 -> SEL1 &= ~(R0|R1|R2|R3);
   P2 -> SEL0 &= ~(R0|R1|R2|R3);
   P2 -> OUT |= (R0|R1|R2|R3); // sets it high
*/
   __enable_irq();
   NVIC -> ISER[1] = 1 << ((PORT5_IRQn) & 31);
}
void PORT5_IRQHandler(void)
{
   P5->IFG &= ~(C1|C2|C3|C4); //clear interrupt flag
   keypad_setkey();
}

void keypad_setkey(void)
{
    //set all low
    P2 -> OUT &= ~(R0|R1|R2|R3);
    uint8_t COL = 0;
    //Multiple if statements that goes through the rows to select the correct key based on //column
    P2 -> OUT = R0;
    delay_ms(25, CLK);
    COL = P5 -> IN & (C1|C2|C3|C4);
    if (COL != 0)
    {
        if ((COL & C1) != 0){
            key = 1; }
        else if ((COL & C2) !=0){
            key = 2;}
        else if ((COL & C3) != 0) {
            key = 3; }
        else {
            key = 65;}
    }

    P2 -> OUT = R1;
    delay_ms(25, CLK);
    COL = P5 -> IN &(C1|C2|C3|C4);
    if (COL != 0)
    {
        if ((COL & C1) != 0){
            key = 4; }
        else if ((COL & C2) != 0) {
            key = 5; }
        else if ((COL & C3) != 0) {
            key = 6; }
        else {
            key = 66;}
    }

    P2 -> OUT = R2;
    delay_ms(25, CLK);
    COL = P5 -> IN &(C1|C2|C3|C4);
    if (COL != 0)
    {
        if ((COL & C1) != 0){
            key = 7; }
        else if ((COL & C2) != 0) {
            key = 8;}
        else if ((COL & C3) != 0) {
            key = 9; }
        else {
            key = 67;}
    }

    P2 -> OUT = R3;
    delay_ms(25, CLK);
    COL = P5 -> IN &(C1|C2|C3|C4);
    if (COL != 0)
    {
        if ((COL & C1) != 0){
            key = 42; }
        else if ((COL & C2) != 0) {
            key = 0; }
        else if ((COL & C3) != 0) {
            key = 35; }
        else {
            key = 46;}
    }

}
uint8_t GetKey(void)
{
    uint8_t tempkey = key;
    key = ResetKey;
    return tempkey;
}

void keypad_testkey(void)
{
 switch(GetKey()) {
        case 1:
            delay_ms(time, CLK);
            write_UART("1");
        break;
        case 2:
            delay_ms(time, CLK);
            write_UART("2");
        break;
        case 3:
            delay_ms(time, CLK);
            write_UART("3");
        break;
        case 65:
            delay_ms(time, CLK);
            write_UART("A");
        break;
        case 4:
            delay_ms(time, CLK);
            write_UART("4");
        break;
        case 5:
            delay_ms(time, CLK);
            write_UART("5");
        break;
        case 6:
            delay_ms(time, CLK);
            write_UART("6");
        break;
        case 66:
            delay_ms(time, CLK);
            write_UART("B");
        break;
        case 7:
            delay_ms(time, CLK);
            write_UART("7");
        break;
        case 8:
            delay_ms(time, CLK);
            write_UART("8");
        break;
        case 9:
            delay_ms(time, CLK);
            write_UART("9");
        break;
        case 67:
            delay_ms(time, CLK);
            write_UART("C");
        break;
        case 42:
            delay_ms(time, CLK);
            write_UART("*");
        break;
        case 0:
            delay_ms(time, CLK);
            write_UART("0");
        break;
        case 35:
            delay_ms(time, CLK);
            write_UART("#");
        break;
        case 46:
            delay_ms(time, CLK);
            write_UART(".");
        break;
        default:
        break;
    }
}




