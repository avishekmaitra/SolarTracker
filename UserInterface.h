/*
 * UserInterface.h
 *
 *  Created on: Feb 23, 2020
 *      Author: Delaney Berger
 */

#ifndef USERINTERFACE_H_
#define USERINTERFACE_H_

typedef enum {
  HOME,
  MANUAL,
  ALGO,
  DEMO
} mode_t ;

void Algorithm_Based(void);
void Demo(void);
void Demo_W2(void);                             //demo for Dolan winter quarter
void Home_Screen(void);
char Manual_Input(void);
void Select_Modes(void);
void Start_Screen(void);
void UI_SetGoalAngle(double newAngle);
void ui_evaluateKey(char manual_angle0, char manual_angle1, char manual_angle2);
double UI_GetGoalAngle(void);
void UI_SetMode(mode_t inputMode);
mode_t UI_GetMode(void);
void UI_RunHomeMode(void);
void UI_RunManualMode(void);
void UI_RunAlgoMode(void);
void UI_RunDemoMode(void);

#endif /* USERINTERFACE_H_ */
