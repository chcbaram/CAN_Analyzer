/*
 * ap.cpp
 *
 *  Created on: Nov 12, 2021
 *      Author: baram
 */




#include "ap.h"
#include "thread/thread.h"


using namespace ap;


static bool is_usb_open = false;



void apInit(void)
{
  threadInit();

  is_usb_open = !usbIsOpen();
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
    esp32Update();
    delay(2);

    if (usbIsOpen() != is_usb_open)
    {
      is_usb_open = usbIsOpen();

      if (is_usb_open == true)
        threadNotify(EVENT_USB_OPEN);
      else
        threadNotify(EVENT_USB_CLOSE);
    }
  }
}


