/*
 * bsp.h
 *
 *  Created on: Nov 12, 2021
 *      Author: baram
 */

#ifndef SRC_BSP_BSP_H_
#define SRC_BSP_BSP_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "def.h"


#include "stm32h7xx_hal.h"


bool bspInit(void);
bool bspDeInit(void);

void delay(uint32_t time_ms);
uint32_t millis(void);

void Error_Handler(void);

void logPrintf(const char *fmt, ...);


#ifdef __cplusplus
}
#endif

#endif /* SRC_BSP_BSP_H_ */
