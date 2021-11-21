/*
 * reset.h
 *
 *  Created on: 2021. 11. 21.
 *      Author: baram
 */

#ifndef SRC_COMMON_HW_INCLUDE_RESET_H_
#define SRC_COMMON_HW_INCLUDE_RESET_H_

#ifdef __cplusplus
extern "C" {
#endif


#include "hw_def.h"


#ifdef _USE_HW_RESET


#define RESET_MODE_RUN_BOOT  (1<<0)



bool resetInit(void);

uint32_t resetGetCount(void);
void resetToBoot(uint32_t timeout);
void resetLog(void);
void resetRunSoftReset(void);
void resetClearFlag(void);
uint8_t resetGetStatus(void);
uint8_t resetGetBits(void);
void resetSetBits(uint8_t data);
void resetSetBootMode(uint8_t boot_mode);
uint8_t resetGetBootMode(void);

#endif


#ifdef __cplusplus
}
#endif

#endif /* SRC_COMMON_HW_INCLUDE_RESET_H_ */
