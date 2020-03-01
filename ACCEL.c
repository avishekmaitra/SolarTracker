/*
 * ACCEL.c
 *
 *  Created on: Feb 24, 2020
 *      Author: AviMaitra
 */

#include "delay.h"
#include "I2C.h"
#include <math.h>
#include "msp.h"
#include <stdint.h>

// All address and values can be found in MMA8452Q data sheet
// Register Addresses
#define STATUS_ADDR     0x00
#define OUT_Z_MSB       0x05
#define XYZ_DATA_CFG    0x0E
#define CTRL_REG1       0x2A
#define CTRL_REG2       0x2B
#define OFFSET_Z_ADDR   0x31

// Register Masks and values
#define OUT_Z_READY     0x04
#define OUT_2G_RANGE    0x00
#define STANDBY_MODE    0x00
#define ACTIVE_MODE     0x03
#define DATA_CONFIG     0x02
#define SW_RESET        0x40
#define AWAKE_CONFIG    0x00

// Misc
#define NUM_OF_SAMPLES  9
#define MEDIAN_INDEX    NUM_OF_SAMPLES/2
#define MAX_VAL         128
#define RAD_TO_ANGLE    57

// Helper Functions
void swap(int *p,int *q)
{
   int t;
   t=*p;
   *p=*q;
   *q=t;
}

// Sorts list a of NUM_OF_SAMPLES length
void sort(int a[])
{
   int i,j;
   for(i = 0;i < NUM_OF_SAMPLES-1;i++)
   {
      for(j = 0;j < NUM_OF_SAMPLES-i-1;j++)
      {
         if(a[j] > a[j+1])
            swap(&a[j],&a[j+1]);
      }
   }
}

// Call only when absolutely necessary (sets all registers to default values)
void ACCEL_Reset (void)
{
    // Reset all registers
    I2C_WriteSingleByte(CTRL_REG2, SW_RESET);
    delay_ms(1, FREQ_48_MHZ);
}

void ACCEL_Init(void)
{
    uint8_t errorFlag = 0;
    // Put in standby mode
    I2C_WriteSingleByte(CTRL_REG1, STANDBY_MODE);
    delay_ms(1, FREQ_48_MHZ);
    if((I2C_ReadSingleByte(CTRL_REG1)&BIT0) != STANDBY_MODE)
    {
        errorFlag = 1;
    }

    // Configure 800Hz, 8-bit sampling
    I2C_WriteSingleByte(CTRL_REG1, DATA_CONFIG);
    delay_ms(1, FREQ_48_MHZ);
    if((I2C_ReadSingleByte(CTRL_REG1)&BIT1) != DATA_CONFIG)
    {
        errorFlag = 1;
    }

    // Configure 2g range
    I2C_WriteSingleByte(XYZ_DATA_CFG, OUT_2G_RANGE);
    delay_ms(1, FREQ_48_MHZ);
    if(I2C_ReadSingleByte(XYZ_DATA_CFG) != OUT_2G_RANGE)
    {
        errorFlag = 1;
    }

    // Configure wake mode
    I2C_WriteSingleByte(CTRL_REG2, AWAKE_CONFIG);
    delay_ms(1, FREQ_48_MHZ);
    if(I2C_ReadSingleByte(CTRL_REG2) != AWAKE_CONFIG)
    {
        errorFlag = 1;
    }

    // Activate accelerometer
    I2C_WriteSingleByte(CTRL_REG1, ACTIVE_MODE);
    delay_ms(1, FREQ_48_MHZ);
    if(I2C_ReadSingleByte(CTRL_REG1) != ACTIVE_MODE)
    {
        errorFlag = 1;
    }

    if(errorFlag)
    {
        // TODO display error on LCD
    }
}

// Assumes accelerometer is completely flat
// Call only when first situating the accelerometer
void ACCEL_Calibrate(void)
{
    // Algorithm based on section 5.1 of AN4069
    // This maybe completely wrong honestly
    int8_t offset_z;
    offset_z = 128-I2C_ReadSingleByte(OUT_Z_MSB);
    I2C_WriteSingleByte(OFFSET_Z_ADDR, offset_z);
}

int8_t ACCEL_GetAngle(void)
{
    int8_t accelData[NUM_OF_SAMPLES];
    int8_t medianData;
    double pVal;
    double radAngle;
    int8_t angle;
    uint8_t i;
    for(i = 0; i < NUM_OF_SAMPLES; i=i+1)
    {
        accelData[i] = I2C_ReadSingleByte(OUT_Z_MSB);
        // Wait until new data ready
        while(!(I2C_ReadSingleByte(STATUS_ADDR)&OUT_Z_READY));
    }

    medianData = accelData[MEDIAN_INDEX];
    pVal = (double)medianData/MAX_VAL;
    radAngle = asin(pVal);
    angle = (int8_t)RAD_TO_ANGLE*radAngle;
    return angle;
}
