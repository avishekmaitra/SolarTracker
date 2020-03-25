/*
 * Relay.c
 *
 *  Created on: Mar 8, 2020
 *      Author: AviMaitra
 */

#include "ACCEL.h"
#include "UserInterface.h"
#include "delay.h"
#include "msp.h"
#include <stdbool.h>

#define THRESHOLD 0.5

enum state
{
    OFF,
    IN,
    OUT
};

static uint8_t relayState = OFF;

void Relay_Init(void)
{
    //Set 6.6 and 6.7 as outputs
    P6 -> DIR |= (BIT6|BIT7);
    P6 -> SEL1 &= ~(BIT6|BIT7);
    P6 -> SEL0 &= ~(BIT6|BIT7);
    P6 -> OUT &= ~(BIT6|BIT7);
}

void Relay_Off(void)
{
    P6 -> OUT &= ~(BIT6|BIT7);
    relayState = OFF;
}

void Relay_In(void)
{
    P6 -> OUT &= ~(BIT6);
    P6 -> OUT |= (BIT7);
    relayState = IN;
}

void Relay_Out(void)
{
    P6 -> OUT |= (BIT6);
    P6 -> OUT &= ~(BIT7);
    relayState = OUT;
}

uint8_t Relay_GetState(void)
{
    return relayState;
}

// Returns true if still moving, false when reaches goal
bool Relay_MoveToGoal(void)
{
    double goalAngle, actualAngle;
    int8_t minThreshold, maxThreshold;
    bool notAtGoal = true;

    goalAngle = UI_GetGoalAngle();
    actualAngle = ACCEL_GetAngle_Double();

    minThreshold = goalAngle - THRESHOLD;
    maxThreshold = goalAngle + THRESHOLD;

    if(actualAngle < minThreshold)
    {
        Relay_In();
        notAtGoal = true;
    }
    else if(actualAngle > maxThreshold)
    {
        Relay_Out();
        notAtGoal = true;
    }
    // Reached goal
    else
    {
        Relay_Off();
        notAtGoal = false;
    }
    return notAtGoal;
}
