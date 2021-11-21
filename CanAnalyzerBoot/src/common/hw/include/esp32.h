/*
 * esp32.h
 *
 *  Created on: 2021. 11. 14.
 *      Author: baram
 */

#ifndef SRC_COMMON_HW_INCLUDE_ESP32_H_
#define SRC_COMMON_HW_INCLUDE_ESP32_H_


#ifdef __cplusplus
 extern "C" {
#endif

#include "hw_def.h"

#ifdef _USE_HW_ESP32



bool esp32Init(void);
void esp32Reset(bool boot);
void esp32SetBaud(uint32_t baud);
void esp32RequestBoot(uint8_t param);
void esp32Update(void);
void esp32DownloadMode(bool enable);
bool esp32IsDownloadMode(void);


#endif


#ifdef __cplusplus
}
#endif


#endif /* SRC_COMMON_HW_INCLUDE_ESP32_H_ */
