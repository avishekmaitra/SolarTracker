/**
 * UART Integration
 *
 * ENGINEERS: AVISHEK MAITRA AND NICOLE SCHWARTZ
 *
 * DESCRIPTION: This component defines and initializes the user interface for the project.
 *
 * INPUT: Key Presses on keyboard
 * OUTPUT: Smooth and intuitive user experience
 *
 * DATE: JUNE 5TH, 2019
 *
 * SPONSOR: CALIFORNIA POLYTECHNIC STATE UNVERSITY - COMPUTER ENGINEERING DEPARTMENT
 */

#include "UART.h"
#include "msp.h"
#include <stdio.h>

//For baud rate to 9600
void initUART(void)
{
    // Configure UART pins
    P1->SEL0 |= (BIT2 | BIT3);

    // Configure UART
    EUSCI_A0->CTLW0 |= EUSCI_A_CTLW0_SWRST; // Put eUSCI in reset
    EUSCI_A0->CTLW0 = EUSCI_A_CTLW0_SWRST |
            EUSCI_A_CTLW0_SSEL__SMCLK;      // Configure eUSCI clock source for SMCLK

    // Baud Rate calculation
    // N: 24000000/(9600) = 2500
    // N/16 = 156.25

    EUSCI_A0->BRW = 156; // UCBRX = 156 from 2500/16 = 156

    // User's Guide Table 24.3.1: UCBRFx = 4
    //UCBRSx=0 b/c N has zero fractional component
    EUSCI_A0->MCTLW = (4 << EUSCI_A_MCTLW_BRF_OFS) |
            EUSCI_A_MCTLW_OS16; // (B/C) N > 16 so need to set this oversample flag

    EUSCI_A0->CTLW0 &= ~EUSCI_A_CTLW0_SWRST; // Initialize eUSCI
    EUSCI_A0->IFG &= ~EUSCI_A_IFG_RXIFG;    // Clear eUSCI RX interrupt flag
    EUSCI_A0->IE |= EUSCI_A_IE_RXIE;        // Enable USCI_A0 RX interrupt

    // Enable global interrupt
    __enable_irq();

    // Enable eUSCIA0 interrupt in NVIC module
    NVIC->ISER[0] = 1 << ((EUSCIA0_IRQn) & 31);

}

//For baud rate of 115200
void initUARTWriteOnly(void)
{
    // Configure UART pins
    P1->SEL0 |= (BIT2 | BIT3);

    // Configure UART
    EUSCI_A0->CTLW0 |= EUSCI_A_CTLW0_SWRST; // Put eUSCI in reset
    EUSCI_A0->CTLW0 = EUSCI_A_CTLW0_SWRST |
    EUSCI_A_CTLW0_SSEL__SMCLK;      // Configure eUSCI clock source for SMCLK

    // Baud Rate calculation
    // 3000000/(115200) = 26.04167
    // Fractional portion = 0.04167


    EUSCI_A0->BRW = 13; // UCBRX = 1 from 26/16 = 1

    // User's Guide Table 24.3.1: UCBRS = 0  from N
    EUSCI_A0->MCTLW = (0 << EUSCI_A_MCTLW_BRF_OFS) |
            EUSCI_A_MCTLW_OS16; // (B/C) > 16 so need to set this oversample flag

    EUSCI_A0->CTLW0 &= ~EUSCI_A_CTLW0_SWRST; // Initialize eUSCI

}

void write_UART(char *string)
{
    //iterate through the char array and print each one
    while(*string != NULL)
    {
        while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
        EUSCI_A0->TXBUF = *(string);
        string ++;
    }
}

void writeDigit(uint8_t myNum)
{
    myNum = myNum + TO_CHAR;
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = myNum;
}

void newLine(void)
{
    //Get to new line
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = 0x0A;
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = 0x0D;
}

void EUSCIA0_IRQHandler(void)
{
    if (EUSCI_A0->IFG & EUSCI_A_IFG_RXIFG)
    {
        char inputFromUART;

        //Read input and store
        inputFromUART = EUSCI_A0 -> RXBUF;

        // Check if the TX buffer is empty first
        while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));

        // Echo the received character back
        EUSCI_A0->TXBUF = inputFromUART;
    }
}

