/*
 * cli.cpp
 *
 *  Created on: 2021. 11. 23.
 *      Author: baram
 */




#include "cli.h"


static const char *thread_name = "cli         ";
static thread_t *thread = NULL;

uint8_t  cli_ch     = _DEF_UART_CLI;
uint32_t cli_baud   = 115200;


static void cliThread(void const *argument);;





bool cliThreadInit(thread_t *p_thread)
{
  bool ret = false;

  thread = p_thread;

  thread->name = thread_name;


  osThreadDef(cliThread, cliThread, _HW_DEF_RTOS_THREAD_PRI_CLI, 0, _HW_DEF_RTOS_THREAD_MEM_CLI);
  if (osThreadCreate(osThread(cliThread), NULL) != NULL)
  {
    ret = true;
    logPrintf("cliThread  \t\t: OK\r\n");
  }
  else
  {
    logPrintf("cliThread  \t\t: Fail\r\n");
  }

  p_thread->is_init = ret;

  return ret;
}

void cliThread(void const *argument)
{
  (void)argument;


  cliOpen(_DEF_UART_CLI, cli_baud);

  while(1)
  {
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
    delay(5);
    thread->hearbeat++;
  }
}
