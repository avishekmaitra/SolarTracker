/*
 * RTC.h
 *
 *  Created on: Jan 2, 2020
 *      Author: AviMaitra
 */

#ifndef RTC_H_
#define RTC_H_

void initRTC(void);
void setRTCYear(uint16_t inputYear);
void setRTCMonth(uint8_t inputMonth);
void startRTC(void);

#endif /* RTC_H_ */
