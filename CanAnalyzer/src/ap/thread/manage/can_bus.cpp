/*
 * can_bus.cpp
 *
 *  Created on: 2021. 12. 10.
 *      Author: baram
 */




#include "can_bus.h"

namespace ap
{

static const char *thread_name = "canBus       ";
static thread_t *thread = NULL;


static void canBusThread(void const *argument);
static bool canBusThreadEvent(Event_t event);



bool canBusThreadInit(thread_t *p_thread)
{
  bool ret = false;

  thread = p_thread;

  thread->name = thread_name;
  thread->onEvent = canBusThreadEvent;


  osThreadDef(canBusThread, canBusThread, _HW_DEF_RTOS_THREAD_PRI_CAN_BUS, 0, _HW_DEF_RTOS_THREAD_MEM_CAN_BUS);
  if (osThreadCreate(osThread(canBusThread), NULL) != NULL)
  {
    ret = true;
    logPrintf("canBusThread  \t\t: OK\r\n");
  }
  else
  {
    logPrintf("canThread  \t\t: Fail\r\n");
  }

  p_thread->is_init = ret;

  return ret;
}

bool canBusThreadEvent(Event_t event)
{
  bool ret = false;

  switch(event)
  {
    default:
      break;
  }

  return ret;
}

void canBusThread(void const *argument)
{
  (void)argument;

  while(1)
  {        
    delay(1000/20);
    thread->hearbeat++;
  }
}


} // namespace ap