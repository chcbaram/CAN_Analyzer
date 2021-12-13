/*
 * cmd_bus.cpp
 *
 *  Created on: 2021. 12. 10.
 *      Author: baram
 */




#include "cmd_bus.h"
#include "cmd_can.h"

namespace ap
{

static const char *thread_name = "cmdBus       ";
static thread_t *thread = NULL;

static uint8_t   cmd_can_ch   = _DEF_UART_USB;
static cmd_can_t cmd_can;
static cmd_can_driver_t cmd_can_driver;

static void cmdBusThread(void const *argument);
static bool cmdBusThreadEvent(Event_t event);

static void     drvInit(cmd_can_driver_t *p_driver);
static uint32_t drvAvailable(void);
static bool     drvFlush(void);
static uint8_t  drvRead(void);
static uint32_t drvWrite(uint8_t *p_data, uint32_t length);





bool cmdBusThreadInit(thread_t *p_thread)
{
  bool ret = false;

  thread = p_thread;

  thread->name = thread_name;
  thread->onEvent = cmdBusThreadEvent;


  osThreadDef(cmdBusThread, cmdBusThread, _HW_DEF_RTOS_THREAD_PRI_CMD_BUS, 0, _HW_DEF_RTOS_THREAD_MEM_CMD_BUS);
  if (osThreadCreate(osThread(cmdBusThread), NULL) != NULL)
  {
    ret = true;
    logPrintf("cmdBusThread  \t\t: OK\r\n");
  }
  else
  {
    logPrintf("cmdThread  \t\t: Fail\r\n");
  }

  p_thread->is_init = ret;

  return ret;
}

bool cmdBusThreadEvent(Event_t event)
{
  bool ret = false;

  switch(event)
  {
    default:
      break;
  }

  return ret;
}

void drvInit(cmd_can_driver_t *p_driver)
{
  p_driver->available = drvAvailable;
  p_driver->flush = drvFlush;
  p_driver->read = drvRead;
  p_driver->write = drvWrite;
}

uint32_t drvAvailable(void)
{
  return uartAvailable(cmd_can_ch);
}

bool drvFlush(void)
{
  return uartFlush(cmd_can_ch);
}

uint8_t drvRead(void)
{
  return uartRead(cmd_can_ch);
}

uint32_t drvWrite(uint8_t *p_data, uint32_t length)
{
  return uartWrite(cmd_can_ch, p_data, length);
}

void cmdBusPrintPacket(cmd_can_packet_t *p_pkt)
{
  logPrintf("pkt ");
  logPrintf("T %02X ", p_pkt->type);
  logPrintf("C %02X ",  p_pkt->cmd);
  logPrintf("A %02X ", p_pkt->addr);

  logPrintf("D ");
  for (int i=0; i<p_pkt->length; i++)
  {
    logPrintf("%02X ", p_pkt->data[i]);
  }
  logPrintf("\n");
}

void cmdBusThread(void const *argument)
{
  uint32_t pre_time;
  (void)argument;
  

  drvInit(&cmd_can_driver);

  cmdCanInit(&cmd_can, &cmd_can_driver);
  cmdCanOpen(&cmd_can);

  pre_time = millis();
  while(1)
  {        
    if (millis()-pre_time >= 1000)
    {
      pre_time = millis();
    }

    if (usbGetType() == USB_CON_CAN)
    {
      if (cmdCanReceivePacket(&cmd_can) == true)
      {
        cmdBusPrintPacket(&cmd_can.rx_packet);

        cmd_can_packet_t *p_pkt = &cmd_can.tx_packet;

        p_pkt->type = PKT_TYPE_CAN;
        p_pkt->cmd  = PKT_CMD_CAN_RECV_CAN0;
        p_pkt->addr = 0x0101;
        p_pkt->length = 2;
        p_pkt->data[0] = 0;
        p_pkt->data[1] = 1;

        cmdCanSendCmdPacket(&cmd_can, p_pkt);  
      }
    }
    delay(2);
    thread->hearbeat++;
  }
}



} // namespace ap