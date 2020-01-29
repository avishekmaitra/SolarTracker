#ifndef RTC_H_
#define RTC_H_
#include <stdint.h>

void initRTC(void);
void setRTCYear(uint16_t inputYear);
uint16_t getRTCYear(void);
void setRTCMonth(uint8_t inputMonth);
uint8_t getRTCMonth(void);
void setRTCDay(uint8_t inputDay);
uint8_t getRTCDay(void);
void setRTCHour(uint8_t inputHour);
uint8_t getRTCHour(void);
void setRTCMinute(uint8_t inputMin);
uint8_t getRTCMinute(void);
uint16_t getCurrentTime(void);
void startRTC(void);

#endif /* RTC_H_ */
