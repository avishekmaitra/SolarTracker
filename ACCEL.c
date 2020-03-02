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
#define OUT_Y_MSB       0x03
#define OUT_X_MSB       0x01
#define XYZ_DATA_CFG    0x0E
#define CTRL_REG1       0x2A
#define CTRL_REG2       0x2B
#define OFFSET_Z_ADDR   0x31
#define OFFSET_Y_ADDR   0x30
#define OFFSET_X_ADDR   0x2F

// Register Masks and values
#define OUT_Z_READY     0x04
#define OUT_Y_READY     0x02
#define OUT_X_READY     0x01
#define OUT_2G_RANGE    0x00
#define STANDBY_MODE    0x00
#define ACTIVE_MODE     0x03
#define DATA_CONFIG     0x02
#define SW_RESET        0x40
#define AWAKE_CONFIG    0x00

// Misc
#define NUM_OF_SAMPLES  9
#define MEDIAN_INDEX    NUM_OF_SAMPLES/2
#define MAX_VAL         64
#define RAD_TO_ANGLE    57.3

// Helper Functions
void swap(int8_t *p,int8_t *q)
{
   int t;
   t=*p;
   *p=*q;
   *q=t;
}

// Sorts list a of NUM_OF_SAMPLES length
void sort(int8_t a[])
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
}

void ACCEL_Init(void)
{
    uint8_t errorFlag = 0;
    // Put in standby mode
    I2C_WriteSingleByte(CTRL_REG1, STANDBY_MODE);
    if((I2C_ReadSingleByte(CTRL_REG1)&BIT0) != STANDBY_MODE)
    {
        errorFlag = 1;
    }

    // Configure 800Hz, 8-bit sampling
    I2C_WriteSingleByte(CTRL_REG1, DATA_CONFIG);
    if((I2C_ReadSingleByte(CTRL_REG1)&BIT1) != DATA_CONFIG)
    {
        errorFlag = 1;
    }

    // Configure 2g range
    I2C_WriteSingleByte(XYZ_DATA_CFG, OUT_2G_RANGE);
    if(I2C_ReadSingleByte(XYZ_DATA_CFG) != OUT_2G_RANGE)
    {
        errorFlag = 1;
    }

    // Configure wake mode
    I2C_WriteSingleByte(CTRL_REG2, AWAKE_CONFIG);
    if(I2C_ReadSingleByte(CTRL_REG2) != AWAKE_CONFIG)
    {
        errorFlag = 1;
    }

    // Activate accelerometer
    I2C_WriteSingleByte(CTRL_REG1, ACTIVE_MODE);
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
    int8_t offset_z, offset_y, offset_x;
    offset_z = 64-I2C_ReadSingleByte(OUT_Z_MSB);
    offset_y = 0-I2C_ReadSingleByte(OUT_Y_MSB);
    offset_x = 0-I2C_ReadSingleByte(OUT_X_MSB);
    I2C_WriteSingleByte(OFFSET_Z_ADDR, offset_z);
    I2C_WriteSingleByte(OFFSET_Y_ADDR, offset_y);
    I2C_WriteSingleByte(OFFSET_X_ADDR, offset_x);
}

int8_t ACCEL_GetAngle(void)
{
    // int8_t accelData_X[NUM_OF_SAMPLES];
    // int8_t accelData_Y[NUM_OF_SAMPLES];
    int8_t accelData_Z[NUM_OF_SAMPLES];
    //int8_t medianData_X;
    //int8_t medianData_Y;
    int8_t medianData_Z;
    double pVal;
    double radians;
    double angle;
    int8_t finalVal;
    //uint8_t i;
    //uint8_t j;
    uint8_t k;

    // Populate X data
    /*for(i = 0; i < NUM_OF_SAMPLES; i=i+1)
    {
        accelData_X[i] = I2C_ReadSingleByte(OUT_X_MSB);
        // Wait until new data ready
        while(!(I2C_ReadSingleByte(STATUS_ADDR)&OUT_X_READY));
    }*/

    // Populate Y data
    /*for(j = 0; j < NUM_OF_SAMPLES; j=j+1)
    {
        accelData_Y[j] = I2C_ReadSingleByte(OUT_Y_MSB);
        // Wait until new data ready
        while(!(I2C_ReadSingleByte(STATUS_ADDR)&OUT_Y_READY));
    }*/

    // Populate Z data
    for(k = 0; k < NUM_OF_SAMPLES; k=k+1)
    {
        accelData_Z[k] = I2C_ReadSingleByte(OUT_Z_MSB);
        // Wait until new data ready
        while(!(I2C_ReadSingleByte(STATUS_ADDR)&OUT_Z_READY));
    }

    //sort(accelData_X);
    //sort(accelData_Y);
    sort(accelData_Z);
    //medianData_X = accelData_X[MEDIAN_INDEX];
    //medianData_Y = accelData_Y[MEDIAN_INDEX];
    medianData_Z = accelData_Z[MEDIAN_INDEX];

    pVal = ((double)medianData_Z/MAX_VAL)*-1;
    radians = acos(pVal);
    angle = (radians*RAD_TO_ANGLE);
    finalVal  = (int8_t)angle;
    return finalVal;
}
