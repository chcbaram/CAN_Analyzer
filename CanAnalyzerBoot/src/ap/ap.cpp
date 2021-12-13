/*
 * ap.cpp
 *
 *  Created on: Nov 12, 2021
 *      Author: baram
 */




#include "ap.h"
#include "boot/boot.h"


#define MAX_BOOT_CH           2



#define BOOT_MODE_LOADER      0
#define BOOT_MODE_CLI         1
#define BOOT_MODE_JUMP_FW     2


uint8_t boot_mode = BOOT_MODE_LOADER;
static cmd_t cmd_boot[MAX_BOOT_CH];

uint8_t  cli_ch     = _DEF_UART_CLI;
uint32_t cli_baud   = 115200;





void apInit(void)
{
  uint8_t boot_param;


  boot_param = resetGetBootMode();

  if (boot_param & RESET_MODE_RUN_BOOT || buttonGetPressed(_DEF_BUTTON1) == true)
  {
    boot_mode = BOOT_MODE_LOADER;
    logPrintf("boot begin...\r\n");

    boot_param &= ~RESET_MODE_RUN_BOOT;
    rtcWriteBackupData(_HW_DEF_RTC_BOOT_MODE, boot_param);

    usbInit();
    usbBegin(USB_CDC_MODE);
    return;
  }

  boot_mode = BOOT_MODE_JUMP_FW;


  logPrintf("jump fw...\r\n");

  if (bootVerifyCrc() != true)
  {
    if (bootVerifyFw() != true)
    {
      logPrintf("fw empty  \t\t: Fail\r\n");
    }
    else
    {
      logPrintf("fw crc    \t\t: Fail\r\n");
    }
    logPrintf("boot begin...\r\n");
    boot_mode = BOOT_MODE_LOADER;
  }
  else
  {
    delay(100);
    bootJumpToFw();
  }

  usbInit();
  usbBegin(USB_CDC_MODE);
}

void apMain(void)
{
  uint32_t pre_time;


  cliOpen(_DEF_UART_CLI, cli_baud);


  cmdInit(&cmd_boot[0]);
  cmdOpen(&cmd_boot[0], _DEF_UART_USB, 57600);

  cmdInit(&cmd_boot[1]);
  cmdOpen(&cmd_boot[1], _DEF_UART_ESP_UART, 1000000);


  pre_time = millis();
  while(1)
  {
    if (boot_mode == BOOT_MODE_LOADER)
    {
      for (int i=0; i<MAX_BOOT_CH; i++)
      {
        if (cmdReceivePacket(&cmd_boot[i]) == true)
        {
          bootProcessCmd(&cmd_boot[i]);
        }
      }

      if (millis()-pre_time >= 100)
      {
        pre_time = millis();
        ledToggle(_DEF_LED1);
        ledToggle(_DEF_LED2);
        ledToggle(_DEF_LED3);
        ledToggle(_DEF_LED4);
      }
    }
    else
    {
      if (millis()-pre_time >= 1000)
      {
        pre_time = millis();
        ledToggle(_DEF_LED1);
        ledToggle(_DEF_LED2);
        ledToggle(_DEF_LED3);
        ledToggle(_DEF_LED4);
      }
    }



    if (usbIsOpen() && usbGetType() == USB_CON_CLI)
    {
      cli_ch = _DEF_UART_USB;
      boot_mode = BOOT_MODE_CLI;
    }
    else
    {
      cli_ch = _DEF_UART_CLI;
      boot_mode = BOOT_MODE_LOADER;
    }

    if (cli_ch != cliGetChannel())
    {
      cliOpen(cli_ch, cli_baud);
    }

    cliMain();
    esp32Update();
  }
}


