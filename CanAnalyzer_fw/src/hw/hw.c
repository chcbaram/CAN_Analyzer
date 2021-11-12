/*
 * hw.c
 *
 *  Created on: Nov 12, 2021
 *      Author: baram
 */




#include "hw.h"





bool hwInit(void)
{
  bspInit();


  cliInit();
  logInit();
  ledInit();
  uartInit();
  uartOpen(_DEF_UART1, 115200);

  logOpen(_DEF_UART1, 115200);
  logPrintf("[ Firmware Begin... ]\r\n");
  logPrintf("Booting..Name \t\t: %s\r\n", _DEF_BOARD_NAME);
  logPrintf("Booting..Ver  \t\t: %s\r\n", _DEF_FIRMWATRE_VERSION);
  logPrintf("Sys Clk       \t\t: %d Mhz\r\n", HAL_RCC_GetSysClockFreq()/1000000);

  logBoot(false);

  return true;
}
