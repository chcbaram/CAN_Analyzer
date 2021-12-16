/*
 * cmd_bus.cpp
 *
 *  Created on: 2021. 12. 10.
 *      Author: baram
 */




#include "cmd_bus.h"
#include "cmd_can.h"
#include "can_bus.h"


namespace ap
{

#define CMD_CAN_PACKET_MAX   32


static const char *thread_name = "cmdBus       ";
static thread_t *thread = NULL;

static uint8_t   cmd_can_ch   = _DEF_UART_USB;
static cmd_can_t cmd_can;
static cmd_can_driver_t cmd_can_driver;
static cmdbus_obj_t cmdbus_obj;

qbuffer_t cmd_can_rx_q;
qbuffer_t cmd_can_tx_q;
static cmd_can_packet_t cmd_can_packet_rx[CMD_CAN_PACKET_MAX];
static cmd_can_packet_t cmd_can_packet_tx[CMD_CAN_PACKET_MAX];

static uint32_t err_cnt_pc2can = 0;
static uint32_t err_cnt_can2pc = 0;

static void cmdBusThread(void const *argument);
static bool cmdBusThreadEvent(Event_t event);
static bool cmdBusThreadBegin(thread_t *p_thread);

static void     drvInit(cmd_can_driver_t *p_driver);
static uint32_t drvAvailable(void);
static bool     drvFlush(void);
static uint8_t  drvRead(void);
static uint32_t drvWrite(uint8_t *p_data, uint32_t length);

static uint32_t cmdBusAvailble(void);
static bool cmdBusRead(cmd_can_packet_t *p_packet);
static bool cmdBusWrite(cmd_can_packet_t *p_packet);

#ifdef _USE_HW_CLI
static void cliCmdBus(cli_args_t *args);
#endif



bool cmdBusThreadInit(thread_t *p_thread)
{
  bool ret = true;

  thread = p_thread;

  thread->name = thread_name;
  thread->begin = cmdBusThreadBegin;
  thread->onEvent = cmdBusThreadEvent;

  cmdbus_obj.available = cmdBusAvailble;
  cmdbus_obj.read = cmdBusRead;
  cmdbus_obj.write = cmdBusWrite;


  qbufferCreateBySize(&cmd_can_rx_q, (uint8_t *)&cmd_can_packet_rx[0], sizeof(cmd_can_packet_t), CMD_CAN_PACKET_MAX);
  qbufferCreateBySize(&cmd_can_tx_q, (uint8_t *)&cmd_can_packet_tx[0], sizeof(cmd_can_packet_t), CMD_CAN_PACKET_MAX);

  p_thread->is_init = ret;

#ifdef _USE_HW_CLI
  cliAdd("cmdBus", cliCmdBus);
#endif
  return ret;
}

bool cmdBusThreadBegin(thread_t *p_thread)
{
  bool ret = false;

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

  p_thread->is_begin = ret;

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

uint32_t cmdBusAvailble(void)
{
  return qbufferAvailable(&cmd_can_rx_q);
}

bool cmdBusRead(cmd_can_packet_t *p_packet)
{
  return qbufferRead(&cmd_can_rx_q, (uint8_t *)p_packet, 1);
}

bool cmdBusWrite(cmd_can_packet_t *p_packet)
{
  return qbufferWrite(&cmd_can_tx_q, (uint8_t *)p_packet, 1);
}

cmdbus_obj_t *cmdBusObj(void)
{
  return &cmdbus_obj;
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
  (void)argument;
  bool ret;
  uint8_t cnt;
  

  drvInit(&cmd_can_driver);

  cmdCanInit(&cmd_can, &cmd_can_driver);
  cmdCanOpen(&cmd_can);

  while(1)
  {        
    if (usbGetType() == USB_CON_CAN)
    {
      // PC -> CAN
      if (cmdCanReceivePacket(&cmd_can) == true)
      {
        ret = qbufferWrite(&cmd_can_rx_q, (uint8_t *)&cmd_can.rx_packet, 1);
        if (ret != true)
        {
          err_cnt_pc2can++;
        }
      }

      // CAN -> PC
      cnt = 0;
      while(qbufferAvailable(&cmd_can_tx_q) > 0)
      {        
        cmd_can_packet_t *p_packet;
        
        if (usbIsOpen() == true)
        {
          p_packet = (cmd_can_packet_t *)qbufferPeekRead(&cmd_can_tx_q);
          ret = cmdCanSendCmdPacket(&cmd_can, p_packet);         
          if (ret != true)
          {
            logPrintf("cmdCanSendCmdPacket() fail\n");
            err_cnt_can2pc++;
          }
        }
        qbufferRead(&cmd_can_tx_q, NULL, 1);

        if (cnt++ > 32)
          break;
      }

      if (cmdCanIsBusy(&cmd_can) == false)
      {
        delay(2);
      }    
    }
    else
    {
      delay(2);
    }
    
    thread->hearbeat++;
  }
}

#ifdef _USE_HW_CLI
void cliCmdBus(cli_args_t *args)
{
  bool ret = false;


  if (args->argc == 1 && args->isStr(0, "info"))
  {
    cliPrintf("err_cnt_can2pc : %d\n", err_cnt_can2pc);
    cliPrintf("err_cnt_pc2pc  : %d\n", err_cnt_pc2can);
    ret = true;
  }

  if (args->argc == 1 && args->isStr(0, "clear"))
  {
    err_cnt_can2pc = 0;
    err_cnt_pc2can = 0;
    ret = true;
  }

  if (ret == false)
  {
    cliPrintf("cmdbus info\n");
    cliPrintf("cmdbus clear\n");
  }
}
#endif


} // namespace ap