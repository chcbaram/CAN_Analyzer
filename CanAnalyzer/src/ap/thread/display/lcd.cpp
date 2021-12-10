/*
 * cli.cpp
 *
 *  Created on: 2021. 11. 23.
 *      Author: baram
 */




#include "lcd.h"


static const char *thread_name = "lcd         ";
static thread_t *thread = NULL;



static void lcdThread(void const *argument);;





bool lcdThreadInit(thread_t *p_thread)
{
  bool ret = false;

  thread = p_thread;

  thread->name = thread_name;


  osThreadDef(lcdThread, lcdThread, _HW_DEF_RTOS_THREAD_PRI_LCD, 0, _HW_DEF_RTOS_THREAD_MEM_LCD);
  if (osThreadCreate(osThread(lcdThread), NULL) != NULL)
  {
    ret = true;
    logPrintf("lcdThread  \t\t: OK\r\n");
  }
  else
  {
    logPrintf("lcdThread  \t\t: Fail\r\n");
  }

  p_thread->is_init = ret;

  return ret;
}

void lcdThread(void const *argument)
{
  (void)argument;
  uint32_t can_rx_cnt[2];
  uint32_t can_tx_cnt[2];


  lcdSetFont(LCD_FONT_HAN);



  while(1)
  {
        
    if (lcdDrawAvailable() == true)
    {
      lcdClearBuffer(black);

      lcdDrawFillRect(0, 0, LCD_WIDTH, 16, white);
      lcdPrintf(35, 0, black, "CanAnalyzer");

      int16_t o_x[2] = {0,  0};
      int16_t o_y[2] = {24, 24+16};

      lcdPrintf(o_x[0], o_y[0], green, "CAN1 T   R");
      lcdPrintf(o_x[1], o_y[1], green, "CAN2 T   R" );


      for (int i=0; i<CAN_MAX_CH; i++)
      {
        lcdDrawRect(8*6+2,  o_y[i]+1, 10, 12, yellow);
        lcdDrawRect(8*10+2, o_y[i]+1, 10, 12, yellow);

        if (can_rx_cnt[i] != canGetRxCount(i))
          lcdDrawFillRect(8*6+2, o_y[i]+1, 10, 12, yellow);

        if (can_tx_cnt[i] != canGetTxCount(i))
          lcdDrawFillRect(8*10+2, o_y[i]+1, 10, 12, yellow);

        can_rx_cnt[i] = canGetRxCount(i);
        can_tx_cnt[i] = canGetTxCount(i);
      }

      lcdRequestDraw();
    }    
    delay(1000/20);
    thread->hearbeat++;
  }
}
