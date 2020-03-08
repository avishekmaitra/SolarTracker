/*
 * UserInterface.h
 *
 *  Created on: Feb 23, 2020
 *      Author: Delaney Berger
 */

#ifndef USERINTERFACE_H_
#define USERINTERFACE_H_

void Algorithm_Based(void);
void Demo(void);
void Demo_W2(void);                             //demo for Dolan winter quarter
void Home_Screen(void);
char Manual_Input(void);
void Select_Modes(void);
void Start_Screen(void);
void ui_setGoalAngle(int8_t newAngle);
void ui_evaluateKey(char manual_angle0, char manual_angle1, char manual_angle2);
int8_t UI_GetGoalAngle(void);

#endif /* USERINTERFACE_H_ */
