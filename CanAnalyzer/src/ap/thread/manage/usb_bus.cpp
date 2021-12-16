/*
 * usb_bus.cpp
 *
 *  Created on: 2021. 12. 14.
 *      Author: baram
 */




#include "usb_bus.h"

namespace ap
{

static const char *thread_name = "usbBus       ";
static thread_t *thread = NULL;


static void usbBusThread(void const *argument);
static bool usbBusThreadEvent(Event_t event);
static bool usbBusThreadBegin(thread_t *p_thread);


bool usbBusThreadInit(thread_t *p_thread)
{
  bool ret = true;

  thread = p_thread;
  
  thread->name = thread_name;
  thread->begin = usbBusThreadBegin;
  thread->onEvent = usbBusThreadEvent;

  p_thread->is_init = ret;

  return ret;
}

bool usbBusThreadBegin(thread_t *p_thread)
{
  bool ret = false;


  osThreadDef(usbBusThread, usbBusThread, _HW_DEF_RTOS_THREAD_PRI_USB_BUS, 0, _HW_DEF_RTOS_THREAD_MEM_USB_BUS);
  if (osThreadCreate(osThread(usbBusThread), NULL) != NULL)
  {
    ret = true;
    logPrintf("usbBusThread  \t\t: OK\r\n");
  }
  else
  {
    logPrintf("usbThread  \t\t: Fail\r\n");
  }

  p_thread->is_begin = ret;

  return ret;
}

bool usbBusThreadEvent(Event_t event)
{
  bool ret = false;

  switch(event)
  {
    default:
      break;
  }

  return ret;
}

void usbBusThread(void const *argument)
{
  (void)argument;
  bool is_usb_open;


  is_usb_open = !usbIsOpen();

  while(1)
  {        
    if (usbIsOpen() != is_usb_open)
    {
      is_usb_open = usbIsOpen();

      if (is_usb_open == true)
        thread->notify(EVENT_USB_OPEN);
      else
        thread->notify(EVENT_USB_CLOSE);
    }    
    delay(10);
    thread->hearbeat++;
  }
}


} // namespace ap