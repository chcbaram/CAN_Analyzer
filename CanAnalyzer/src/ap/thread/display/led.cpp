/*
 * led.cpp
 *
 *  Created on: 2021. 12. 13.
 *      Author: baram
 */




#include "led.h"


namespace ap
{

static const char *thread_name = "led         ";
static thread_t *thread = NULL;

static uint32_t can_rx_cnt[2] = {0, };
static uint32_t can_tx_cnt[2] = {0, };


static void ledThread(void const *argument);;

static void updateCan(void);
static void updateRun(void);
static void updateMode(void);





bool ledThreadInit(thread_t *p_thread)
{
  bool ret = false;

  thread = p_thread;

  thread->name = thread_name;


  osThreadDef(ledThread, ledThread, _HW_DEF_RTOS_THREAD_PRI_LED, 0, _HW_DEF_RTOS_THREAD_MEM_LED);
  if (osThreadCreate(osThread(ledThread), NULL) != NULL)
  {
    ret = true;
    logPrintf("ledThread  \t\t: OK\r\n");
  }
  else
  {
    logPrintf("ledThread  \t\t: Fail\r\n");
  }

  p_thread->is_init = ret;

  return ret;
}

void updateCan(void)
{
  enum
  {
    STATE_IDLE,
    STATE_LED_ON,
    STATE_LED_OFF,
  };

  const uint8_t led_on_time  = 4;
  const uint8_t led_off_time = 4;
  const uint8_t led_ch[CAN_MAX_CH] = {_DEF_LED_CAN1, _DEF_LED_CAN2};

  static uint8_t state[CAN_MAX_CH] = {0, };
  static uint8_t led_on[CAN_MAX_CH] = {0, };
  static uint8_t led_time[CAN_MAX_CH] = {0, };


  for (int i=0; i<CAN_MAX_CH; i++)
  {
    if (canGetRxCount(i) != can_rx_cnt[i])
      led_on[i] = 1;
    if (canGetTxCount(i) != can_tx_cnt[i])
      led_on[i] = 1;

    switch(state[i])
    {
      case STATE_IDLE:
        if (led_on[i] > 0)
        {
          led_on[i] = 0;
          state[i] = STATE_LED_ON;
          led_time[i] = led_on_time;
          ledOn(led_ch[i]);
        }
        break;

      case STATE_LED_ON:
        if (led_time[i] > 0)
          led_time[i]--;

        if (led_time[i] == 0)
        {
          state[i] = STATE_LED_OFF;
          led_time[i] = led_off_time;
          ledOff(led_ch[i]);
        }
        break;

      case STATE_LED_OFF:
        if (led_time[i] > 0)
          led_time[i]--;

        if (led_time[i] == 0)
          state[i] = STATE_IDLE;
        break;
    }

    can_rx_cnt[i] = canGetRxCount(i);
    can_tx_cnt[i] = canGetTxCount(i);
  }
}

void updateRun(void)
{
  static uint32_t pre_time;

  if (millis()-pre_time >= 500)
  {
    pre_time = millis();

    ledToggle(_DEF_LED_RUN);
  }
}

void updateMode(void)
{
  if (usbGetType() == USB_CON_CAN)
    ledOn(_DEF_LED_MODE);
  else
    ledOff(_DEF_LED_MODE);
}

void ledThread(void const *argument)
{
  (void)argument;



  while(1)
  {
    updateCan();
    updateRun();
    updateMode();

    delay(10);
    thread->hearbeat++;
  }
}

} // namespace ap