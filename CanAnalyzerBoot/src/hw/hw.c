/*
 * hw.c
 *
 *  Created on: Nov 12, 2021
 *      Author: baram
 */




#include "hw.h"


const __attribute__((section(".version"))) firm_version_t boot_ver =
    {
        _DEF_BOOT_VERSION,
        _DEF_BOARD_NAME
    };


bool hwInit(void)
{
  bspInit();


  resetInit();
  cliInit();
  logInit();
  ledInit();
  buttonInit();
  gpioInit();
  uartInit();
  uartOpen(_DEF_UART1, 115200);
  uartOpen(_DEF_UART2, 115200);
  uartOpen(_DEF_UART3, 115200);
  uartOpen(_DEF_UART4, 115200);


  logOpen(_DEF_UART1, 115200);
  logPrintf("\n\n[ Bootloader Begin... ]\r\n");
  logPrintf("Booting..Name \t\t: %s\r\n", boot_ver.name);
  logPrintf("Booting..Ver  \t\t: %s\r\n", boot_ver.version);
  logPrintf("Sys Clk       \t\t: %d Mhz\r\n", HAL_RCC_GetSysClockFreq()/1000000);

  resetLog();
  rtcInit();
  qspiInit();
  flashInit();

  esp32Init();

  logBoot(false);

  return true;
}
