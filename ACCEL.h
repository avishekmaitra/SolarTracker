/*
 * ACCEL.h
 *
 *  Created on: Feb 24, 2020
 *      Author: AviMaitra
 */

#ifndef ACCEL_H_
#define ACCEL_H_

void ACCEL_Reset (void);
void ACCEL_Init(void);
void ACCEL_Calibrate(void);
int8_t ACCEL_GetAngle(void);


#endif /* ACCEL_H_ */
