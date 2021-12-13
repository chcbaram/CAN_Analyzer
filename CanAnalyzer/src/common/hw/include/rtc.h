/*
 * rtc.h
 *
 *  Created on: 2021. 11. 21.
 *      Author: baram
 */

#ifndef SRC_COMMON_HW_INCLUDE_RTC_H_
#define SRC_COMMON_HW_INCLUDE_RTC_H_


#ifdef __cplusplus
 extern "C" {
#endif

#include "hw_def.h"


#ifdef _USE_HW_RTC


#define RTC_CFG_DATA_1    HW_RTC_CFG_DATA_1
#define RTC_CFG_DATA_2    HW_RTC_CFG_DATA_2


bool rtcInit(void);
bool rtcIsInit(void);

time_t rtcGetTime();
void rtcSetTime(time_t time_data);
void rtcWriteBackupData(uint32_t index, uint32_t data);
uint32_t rtcReadBackupData(uint32_t index);
uint32_t rtcGetSecondsFromPower(void);
uint32_t rtcGetSecondsFromReset(void);
uint8_t resetGetStatus(void);
uint8_t resetGetBits(void);

#endif

#ifdef __cplusplus
 }
#endif


#endif /* SRC_COMMON_HW_INCLUDE_RTC_H_ */
