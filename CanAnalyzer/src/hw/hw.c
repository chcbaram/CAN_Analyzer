/*
 * hw.c
 *
 *  Created on: Nov 12, 2021
 *      Author: baram
 */




#include "hw.h"


extern uint32_t _flash_tag_addr;
extern uint32_t __isr_vector_addr;


__attribute__((section(".tag"))) firm_tag_t firm_tag =
    {
        .magic_number = 0xAAAA5555,

        //-- fw info
        //
        .addr_tag = (uint32_t)&_flash_tag_addr,
        .addr_fw  = (uint32_t)&__isr_vector_addr,
        .size_tag = 1024,


        //-- tag info
        //
    };


__attribute__((section(".version"))) firm_version_t firm_ver =
    {
        _DEF_FIRMWATRE_VERSION,
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
  logPrintf("\r\n[ Firmware Begin... ]\r\n");
  logPrintf("Booting..Name \t\t: %s\r\n", _DEF_BOARD_NAME);
  logPrintf("Booting..Ver  \t\t: %s\r\n", _DEF_FIRMWATRE_VERSION);
  logPrintf("Sys Clk       \t\t: %d Mhz\r\n", HAL_RCC_GetSysClockFreq()/1000000);

  rtcInit();
  logPrintf("ResetBits \t\t: 0x%02X\n", (int)rtcReadBackupData(_HW_DEF_RTC_RESET_SRC));

  resetLog();
  qspiInit();
  flashInit();
  usbInit();
  usbBegin(USB_CDC_MODE);

  esp32Init();

  logBoot(false);

  return true;
}
