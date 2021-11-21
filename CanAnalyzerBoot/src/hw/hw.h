/*
 * hw.h
 *
 *  Created on: Nov 12, 2021
 *      Author: baram
 */

#ifndef SRC_HW_HW_H_
#define SRC_HW_HW_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "hw_def.h"


#include "led.h"
#include "uart.h"
#include "log.h"
#include "cli.h"
#include "button.h"
#include "gpio.h"
#include "cdc.h"
#include "usb.h"
#include "esp32.h"
#include "qspi.h"
#include "flash.h"
#include "reset.h"
#include "rtc.h"
#include "cmd.h"


bool hwInit(void);


#ifdef __cplusplus
}
#endif

#endif /* SRC_HW_HW_H_ */
