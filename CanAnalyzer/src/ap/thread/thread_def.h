/*
 * thread_def.h
 *
 *  Created on: 2021. 12. 10.
 *      Author: baram
 */

#ifndef SRC_AP_THREAD_DEF_H_
#define SRC_AP_THREAD_DEF_H_


typedef enum E_EVENT
{
  EVENT_USB_CONNECTED,
  EVENT_USB_DISCONNECTED,
  EVENT_USB_OPEN,
  EVENT_USB_CLOSE,
} Event_t;

typedef enum
{
  THREAD_ID_INFO,
  THREAD_ID_CLI,
  THREAD_ID_LCD,
  THREAD_ID_CAN_BUS,
  THREAD_ID_CMD_BUS,
  THREAD_ID_LED,
  THREAD_ID_USB_BUS,
  THREAD_ID_MAX
} ThreadId_t;



typedef struct thread_t_ thread_t;

typedef struct thread_t_
{
  bool is_init;
  bool is_begin;

  const char *name;
  uint32_t freq;
  uint32_t hearbeat;

  thread_t *list;

  bool (*init)(thread_t *p_thread);
  bool (*begin)(thread_t *p_thread);

  bool (*notify)(Event_t event);
  bool (*onEvent)(Event_t event);

} thread_t;





#endif