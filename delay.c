/**
 * ACCURATE DELAY GENERATOR
 *
 * ENGINEERS: NICOLE SCHWARTZ AND AVISHEK MAITRA
 *
 * DESCRIPTION: This file creates software busy-wait delays used.
 *
 * INPUT: Clock Frequency and Count
 * OUTPUT: Delay corresponding to the count and its unit
 *
 * DATE: APRIL 17TH, 2019
 *
 * SPONSOR: CALIFORNIA POLYTECHNIC STATE UNIVERSITY - COMPUTER ENGINEERING DEPARTMENT
 */

#include "msp.h"
#include "delay.h"

void delay_ms(int count, int freq)
{
        //Creates a delay in milliseconds. For example, if the count is 10, a 10 mS delay would be created
        //Takes the frequency and adjust the total count while using the MS constant as a linear approx
        uint16_t i;
        uint16_t j;

        if(freq == FREQ_1p5_MHZ)
        {
            count = count/2;
        }
        else if (freq == FREQ_6_MHZ)
        {
            count = count*2;
        }
        else if (freq == FREQ_12_MHZ)
        {
            count = count*4;
        }
        else if (freq == FREQ_24_MHZ)
        {
            count = count*8;
        }
        else if (freq == FREQ_48_MHZ)
        {
            count = count*16;
        }
        else
        {
            count = count;
        }

        //Uses a busy-wait loop to create a very accurate delay
        for (j = 0; j < count; j++)
        {
            for (i = MS; i > 0; i--);
        }
}

void delay_us(int count, int freq)
{
        //Uses the assembly delay function to create a quicker, well-controlled delay
        //This delay is in microseconds. For example, if the count is 10, the delay would be 10 uS.
        //NOTE: CERTAIN COMBINATIONS OF FREQUENCY AND DELAYS ARE IMPOSSIBLE
        if (freq == FREQ_48_MHZ) //Only work for multiples of around 3
        {
            int i;
            int myCount = count/3;
            for(i=0; i<myCount; i++)
            {
                __delay_cycles(LINEAR_US);
            }
        }
        else if (freq == FREQ_1p5_MHZ) //only works with delays of multiples of 100
        {
            int i;
            int myCount = count/100;
            for(i=0; i<myCount; i++)
            {
                __delay_cycles(LINEAR_US);
            }
        }
        else if (freq == FREQ_3_MHZ) //only works with delays of multiples of 50
        {
            int i;
            int myCount = count/50;
            for(i=0; i<myCount; i++)
            {
                __delay_cycles(LINEAR_US);
            }
        }
        else if (freq == FREQ_6_MHZ) //Only works with multiples of 25
        {
            int i;
            int myCount = count/25;
            for(i=0; i<myCount; i++)
            {
                __delay_cycles(LINEAR_US);
            }
        }
        else if (freq == FREQ_12_MHZ) //Only works with multiples of 12
        {
            int i;
            int myCount = count/12;
            for(i=0; i<myCount; i++)
            {
                __delay_cycles(LINEAR_US);
            }
        }

        else if (freq == FREQ_24_MHZ)  //Only work with multiples of close to 6
        {
            int i;
            int myCount = count/6;
            for(i=0; i<myCount; i++)
            {
                __delay_cycles(LINEAR_US);
            }
        }
        else
        {
            int i;
            for(i=0; i<count; i++)
            {
                __delay_cycles(1);
            }
        }
}

void set_DCO(int freq)
{
    // change DC0 from default of 3MHz to 12MHz.
    CS->KEY = CS_KEY_VAL;
    CS->CTL0 = 0;

    //Changes based on input
    if(freq == FREQ_3_MHZ)
    {
        CS->CTL0 = CS_CTL0_DCORSEL_1;

        // select clock sources
        CS->CTL1 = CS_CTL1_SELA_2 | CS_CTL1_SELS_3 | CS_CTL1_SELM_3;
        CS->KEY = 0; // lock the CS registers
    }
    else if(freq == FREQ_6_MHZ)
    {
        CS->CTL0 = CS_CTL0_DCORSEL_2;

        // select clock sources
        CS->CTL1 = CS_CTL1_SELA_2 | CS_CTL1_SELS_3 | CS_CTL1_SELM_3;
        CS->KEY = 0; // lock the CS registers
    }
    else if(freq == FREQ_12_MHZ)
    {
        CS->CTL0 = CS_CTL0_DCORSEL_3;

        // select clock sources
        CS->CTL1 = CS_CTL1_SELA_2 | CS_CTL1_SELS_3 | CS_CTL1_SELM_3;
        CS->KEY = 0; // lock the CS registers
    }
    else if(freq == FREQ_24_MHZ)
    {
        CS->CTL0 = CS_CTL0_DCORSEL_4;

        // select clock sources
        CS->CTL1 = CS_CTL1_SELA_2 | CS_CTL1_SELS_3 | CS_CTL1_SELM_3;
        CS->KEY = 0; // lock the CS registers
    }
    else if(freq == FREQ_48_MHZ)
    {
        /* Transition to VCORE Level 1: AM0_LDO --> AM1_LDO */
        while ((PCM->CTL1 & PCM_CTL1_PMR_BUSY)); PCM->CTL0 = PCM_CTL0_KEY_VAL | PCM_CTL0_AMR_1;
        while ((PCM->CTL1 & PCM_CTL1_PMR_BUSY));

        /* Configure Flash wait-state to 1 for both banks 0 & 1 */
        FLCTL->BANK0_RDCTL = (FLCTL->BANK0_RDCTL &
             ~(FLCTL_BANK0_RDCTL_WAIT_MASK)) | FLCTL_BANK0_RDCTL_WAIT_1;
        FLCTL->BANK1_RDCTL  = (FLCTL->BANK0_RDCTL &
             ~(FLCTL_BANK1_RDCTL_WAIT_MASK)) | FLCTL_BANK1_RDCTL_WAIT_1;

        /* Configure DCO to 48MHz, ensure MCLK uses DCO as source*/
        CS->CTL0 = CS_CTL0_DCORSEL_5;  // Set DCO to 48MHz
        /* Select MCLK = DCO, no divider */
        CS->CTL1 = CS->CTL1 & ~(CS_CTL1_SELM_MASK | CS_CTL1_DIVM_MASK) |
            CS_CTL1_SELM_3;
        CS->KEY = 0;              // Lock CS module from unintended accesses
    }
    else
    {
        CS->CTL0 = CS_CTL0_DCORSEL_0;

        // select clock sources
        CS->CTL1 = CS_CTL1_SELA_2 | CS_CTL1_SELS_3 | CS_CTL1_SELM_3;
        CS->KEY = 0; // lock the CS registers
    }
}

void delay_us_48_MHZ(int count) //Use for 1 and 2 uS delays.
{
    int i;
    int myCount = count/2;  //only works with multiples of 2
    for(i=0; i<myCount; i++)
    {
        __delay_cycles(50);
    }
}
