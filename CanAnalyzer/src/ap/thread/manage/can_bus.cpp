/*
 * can_bus.cpp
 *
 *  Created on: 2021. 12. 10.
 *      Author: baram
 */




#include "can_bus.h"
#include "cmd_bus.h"

namespace ap
{

static const char *thread_name = "canBus       ";
static thread_t *thread = NULL;
static osMutexId mutex_lock;
static cmd_can_packet_t cmd_packet_rx;
static cmd_can_packet_t cmd_packet_tx;
static can_msg_t can_msg_rx;
static can_msg_t can_msg_tx;

static void canBusThread(void const *argument);
static bool canBusThreadEvent(Event_t event);


bool canBusThreadInit(thread_t *p_thread)
{
  bool ret = false;

  thread = p_thread;

  thread->name = thread_name;
  thread->onEvent = canBusThreadEvent;

  osMutexDef(mutex_lock);
  mutex_lock = osMutexCreate (osMutex(mutex_lock));
  
  canOpen(_DEF_CAN1, CAN_NORMAL, CAN_CLASSIC, CAN_1M, CAN_2M);
  canOpen(_DEF_CAN2, CAN_NORMAL, CAN_CLASSIC, CAN_1M, CAN_2M);


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
  bool ret;
  uint32_t cnt;
  can_info_t can_info;

  while(1)
  {        
    canLock();

    if (usbGetType() == USB_CON_CAN)
    {
      // PC -> CAN    
      cnt = 0;
      while(cmdBusObj()->available())
      {
        ret = cmdBusObj()->read(&cmd_packet_rx);
        if (ret == true)
        {               
          switch(cmd_packet_rx.cmd)
          {
            case PKT_CMD_CAN_RECV_CAN0:
              logPrintf("PC -> CAN1\n");  

              canGetInfo(_DEF_CAN1, &can_info);
              can_msg_tx.id_type = CAN_EXT;
              can_msg_tx.frame   = can_info.frame;
              can_msg_tx.id      = cmd_packet_rx.addr;
              can_msg_tx.dlc     = canGetDlc(cmd_packet_rx.length);
              memcpy(can_msg_tx.data, cmd_packet_rx.data, canGetLen(can_msg_tx.dlc));
              if (canMsgWrite(_DEF_CAN1, &can_msg_tx, 50) != true)
              {
                logPrintf("canMsgWrite(_DEF_CAN1) fail\n");
              }
              break;

            case PKT_CMD_CAN_RECV_CAN1:
              logPrintf("PC -> CAN2\n");  

              canGetInfo(_DEF_CAN1, &can_info);              
              can_msg_tx.id_type = CAN_EXT;
              can_msg_tx.frame   = can_info.frame;
              can_msg_tx.id      = cmd_packet_rx.addr;
              can_msg_tx.dlc     = canGetDlc(cmd_packet_rx.length);
              memcpy(can_msg_tx.data, cmd_packet_rx.data, canGetLen(can_msg_tx.dlc));
              if (canMsgWrite(_DEF_CAN2, &can_msg_tx, 50) != true)
              {
                logPrintf("canMsgWrite(_DEF_CAN2) fail\n");
              }
              break;

            default:
              logPrintf("Unknown CMD\n");
              break;
          }                    
        }

        if (cnt++ > 8)
          break;
      }

      // CAN -> PC
      for (int i=0; i<CAN_MAX_CH; i++)
      {
        cnt = 0;
        while(canMsgAvailable(i) > 0)
        {
          ret = canMsgRead(i, &can_msg_rx);
          if (ret == true)
          {
            logPrintf("CAN -> PC\n");

            cmd_packet_tx.type = PKT_TYPE_CAN;
            cmd_packet_tx.cmd  = (i == 0) ? PKT_CMD_CAN_RECV_CAN0 : PKT_CMD_CAN_RECV_CAN1;
            cmd_packet_tx.addr = can_msg_rx.id;
            cmd_packet_tx.length = canGetLen(can_msg_rx.dlc);
            memcpy(cmd_packet_tx.data, can_msg_rx.data, cmd_packet_tx.length);

            if (cmdBusObj()->write(&cmd_packet_tx) != true)
            {
              logPrintf("cmdBusObj()->write() %d fail\n", i);
            }
          }

          if (cnt++ > 8)
            break;
        }
      }
    }

    canUnLock();
    delay(5);
    thread->hearbeat++;
  }
}


} // namespace ap