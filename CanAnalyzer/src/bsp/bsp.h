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
#include "rtos.h"


bool bspInit(void);
bool bspDeInit(void);

void delay(uint32_t time_ms);
uint32_t millis(void);

void Error_Handler(void);

void logPrintf(const char *fmt, ...);

#if 1
#define LOG_ERROR_(fmt, ...) \
    logPrintf("%s:%d:error: " fmt "%s\n", __FILE__, __LINE__, __VA_ARGS__)
#define LOG_ERROR(...) LOG_ERROR_(__VA_ARGS__, "")
#else
#define LOG_ERROR(...)
#endif

#ifdef __cplusplus
}
#endif

#endif /* SRC_BSP_BSP_H_ */
