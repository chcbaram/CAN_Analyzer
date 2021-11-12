/*
 * ap.cpp
 *
 *  Created on: Nov 12, 2021
 *      Author: baram
 */




#include "ap.h"





void apInit(void)
{
  cliOpen(_DEF_UART1, 115200);
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
    }

    cliMain();
  }
}