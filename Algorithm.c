/*
 * Algorithm.c
 *
 *  Created on: May 8, 2020
 *      Author: AviMaitra
 */
#include "Algorithm.h"

#define TIME_OFFSET 300
#define ARRAY_SIZE 900

static double angleArray[ARRAY_SIZE] = {0};

// PLACE HELPER FUNCTIONS HERE

double Algorithm_GetAngle(uint16_t currentTime)
{
    uint16_t newIndex;

    newIndex = currentTime - TIME_OFFSET;
    return angleArray[newIndex];
}

void populateAngleArray(uint16_t currentDay)
{
    // Should be called when it is 12:01 AM.
    // TODO Populate angle array with new angles for the array
}

