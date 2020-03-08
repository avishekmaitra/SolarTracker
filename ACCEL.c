/*
 * ACCEL.c
 *
 *  Created on: Feb 24, 2020
 *      Author: AviMaitra
 */

#include "ACCEL.h"
#include "delay.h"
#include "I2C.h"
#include "LCD.h"
#include <math.h>
#include "msp.h"
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

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
#define ACTIVE_MODE     0x01
#define DATA_CONFIG     0x00
#define SW_RESET        0x40
#define AWAKE_CONFIG    0x00

// Misc
#define NUM_OF_SAMPLES  9
#define MEDIAN_INDEX    NUM_OF_SAMPLES/2
#define MAX_VAL         1024
#define RAD_TO_ANGLE    57.29577951
#define SHIFT_4         4
#define MAX_ANGLE       90
#define MIN_ANGLE       -90
#define NORM_MAX        0.999
#define NORM_MIN        -0.999
#define MAX_LENGTH      4
#define TO_CHAR         0x30

// Static variables
static int8_t offset_z = 0;

// Helper Functions
void swap(int16_t *p,int16_t *q)
{
   int t;
   t=*p;
   *p=*q;
   *q=t;
}

// Sorts list a of NUM_OF_SAMPLES length
void sort(int16_t a[])
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

// Initialize ACCEL – Call after I2C initialization
void ACCEL_Init(void)
{
    uint8_t errorFlag = 0;

    // Put in standby mode
    I2C_WriteSingleByte(CTRL_REG1, STANDBY_MODE);
    if((I2C_ReadSingleByte(CTRL_REG1)&BIT0) != STANDBY_MODE)
    {
        errorFlag = 1;
    }

    // Configure 800Hz, 12-bit sampling
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
        LCD_Clear();
        LCD_Write_L1("ACCEL_Init() error");
    }
}

// Assumes accelerometer is completely flat
// Call only when first situating the accelerometer
void ACCEL_Calibrate(void)
{
    // Calibrate with solar panel completely vertical (accelerometer flat)
    int16_t preData;
    int16_t accelData_Z[NUM_OF_SAMPLES];
    int16_t medianData_Z;

    uint8_t k;

    // Populate Z data
    for(k = 0; k < NUM_OF_SAMPLES; k=k+1)
    {
        preData = I2C_ReadMultiByte(OUT_Z_MSB);
        accelData_Z[k] = (preData >> SHIFT_4);
        // Wait until new data ready
        while(!(I2C_ReadSingleByte(STATUS_ADDR)&OUT_Z_READY));
    }

    sort(accelData_Z);

    medianData_Z = accelData_Z[MEDIAN_INDEX];
    offset_z = MAX_VAL-medianData_Z;
}

int8_t ACCEL_GetAngle_Int(void)
{
    int16_t accelData_Z[NUM_OF_SAMPLES];
    int16_t preData;
    int16_t medianData_Z;

    double pVal;
    double radians;
    double angle;

    int8_t finalVal;
    uint8_t k;

    // Populate Z data
    for(k = 0; k < NUM_OF_SAMPLES; k=k+1)
    {
        preData = I2C_ReadMultiByte(OUT_Z_MSB);
        accelData_Z[k] = (preData >> SHIFT_4);
        // Wait until new data ready
        while(!(I2C_ReadSingleByte(STATUS_ADDR)&OUT_Z_READY));
    }

    sort(accelData_Z);

    medianData_Z = accelData_Z[MEDIAN_INDEX] + offset_z;

    pVal = ((double)medianData_Z/MAX_VAL);

    // Account for max/min exemption
    if(pVal > NORM_MAX)
    {
        finalVal = MAX_ANGLE;
    }
    else if(pVal < NORM_MIN)
    {
        finalVal  = MIN_ANGLE;
    }
    else
    {
        radians = asin(pVal);
        angle = (radians*RAD_TO_ANGLE);
        finalVal  = (int8_t)angle;
    }

    return finalVal;
}

char* ACCEL_GetAngle_String(void)
{
    int8_t angleVal;
    angleVal = ACCEL_GetAngle_Int();

    // Make space for angle string
    uint8_t size;
    size = MAX_LENGTH;
    char* str = (char*)malloc(sizeof(char)*size);
    *(str+0) = '\0';
    *(str+1) = '\0';
    *(str+2) = '\0';
    *(str+3) = '\0';

    if(angleVal<=90 && angleVal >=10)
    {
        *(str+0) = (angleVal/10) + TO_CHAR;
        *(str+1) = (angleVal%10) + TO_CHAR;
    }
    else if(angleVal<10 && angleVal>=0)
    {
        *(str+0) = (angleVal%10) + TO_CHAR;;
    }
    else if(angleVal<0 && angleVal>-10)
    {
        angleVal = angleVal*-1;
        *(str+0) = '-';
        *(str+1) = (angleVal%10) + TO_CHAR;
    }
    else if(angleVal>=-90)
    {
        angleVal = angleVal*-1;
        *(str+0) = '-';
        *(str+1) = (angleVal/10) + TO_CHAR;
        *(str+2) = (angleVal%10) + TO_CHAR;
    }
    else
    {
        *(str+0) = 'I';
        *(str+1) = 'N';
        *(str+2) = 'V';
    }

    return str;
}
