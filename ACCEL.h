/*
 * ACCEL.h
 *
 *  Created on: Feb 24, 2020
 *      Author: AviMaitra
 */

#ifndef ACCEL_H_
#define ACCEL_H_
#include <stdint.h>

void ACCEL_Reset (void);
void ACCEL_Init(void);
void ACCEL_Calibrate(void);
int8_t ACCEL_GetAngle_Int(void);
char* ACCEL_GetAngle_String(void);


#endif /* ACCEL_H_ */
