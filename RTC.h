#ifndef RTC_H_
#define RTC_H_
#include <stdint.h>
#include <stdbool.h>

void RTC_Init(void);
void RTC_Start(void);
void RTC_SetYear(uint16_t inputYear);
uint16_t RTC_GetYear(void);
void RTC_SetMonth(uint8_t inputMonth);
uint8_t RTC_GetMonth(void);
void RTC_SetDay(uint8_t inputDay);
uint8_t RTC_GetDay(void);
void RTC_SetHour(uint8_t inputHour);
uint8_t RTC_GetHour(void);
void RTC_SetMinute(uint8_t inputMin);
uint8_t RTC_GetMinute(void);
uint16_t RTC_GetCurrentTime(void);
uint16_t RTC_GetCurrentDay(void);
void RTC_EnableInterrupt(void);
void RTC_DisableInterrupt(void);
void RTC_SetEventFlag(void);
void RTC_ResetEventFlag(void);
bool RTC_HasEventOccured(void);

#endif /* RTC_H_ */
