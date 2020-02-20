

#ifndef KEYPAD_H_
#define KEYPAD_H_
#include <stdint.h>

#define RESETKEY 73
#define TIMEUART 30
#define TIMESETKEY 1

//pin2
#define R0 BIT3
#define R1 BIT4
#define R2 BIT5
#define R3 BIT6

//pin5
#define C1 BIT0
#define C2 BIT1
#define C3 BIT2
#define C4 BIT6

void Keypad_Init(void);
void keypad_setkey(void);
uint8_t Keypad_GetKey(void);
void keypad_testkey(void);


#endif /* KEYPAD_H_ */
