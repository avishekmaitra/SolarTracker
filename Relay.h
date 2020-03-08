/*
 * Relay.h
 *
 *  Created on: Mar 8, 2020
 *      Author: AviMaitra
 */

#ifndef RELAY_H_
#define RELAY_H_

#include <stdbool.h>

void Relay_Init(void);
void Relay_Off(void);
void Relay_In(void);
void Relay_Out(void);
uint8_t Relay_GetState(void);
bool Relay_MoveToGoal(void);


#endif /* RELAY_H_ */
