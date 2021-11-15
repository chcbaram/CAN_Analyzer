/*
 * ap.cpp
 *
 *  Created on: Nov 12, 2021
 *      Author: baram
 */




#include "ap.h"



uint8_t  cli_ch     = _DEF_UART_CLI;
uint32_t cli_baud   = 115200;


void apInit(void)
{
  cliOpen(_DEF_UART_CLI, cli_baud);
}

void apMain(void)
{
  uint32_t pre_time;


  pre_time = millis();
  while(1)
  {
    if (millis()-pre_time >= 500)
    {
      pre_time = millis();

      ledToggle(_DEF_LED1);
      ledToggle(_DEF_LED2);
      ledToggle(_DEF_LED3);
      ledToggle(_DEF_LED4);
    }

    if (usbIsOpen() && usbGetType() == USB_CON_CLI)
    {
      cli_ch = _DEF_UART_USB;
    }
    else
    {
      cli_ch = _DEF_UART_CLI;
    }

    if (cli_ch != cliGetChannel())
    {
      cliOpen(cli_ch, cli_baud);
    }

    cliMain();
    esp32Update();
  }
}


