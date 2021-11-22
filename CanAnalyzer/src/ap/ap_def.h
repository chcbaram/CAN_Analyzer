/*
 * ap_def.h
 *
 *  Created on: Nov 12, 2021
 *      Author: baram
 */

#ifndef SRC_AP_AP_DEF_H_
#define SRC_AP_AP_DEF_H_


#include "hw.h"


typedef enum E_EVENT
{
  EVENT_USB_CONNECTED,
  EVENT_USB_DISCONNECTED,
  EVENT_USB_OPEN,
  EVENT_USB_CLOSE,
} Event_t;



typedef struct thread_t_ threat_t;

typedef struct thread_t_
{
  bool is_init;
  bool is_start;

  const char *name;
  uint32_t freq;
  uint32_t hearbeat;

  threat_t *list;

  bool (*notify)(Event_t event);


  bool (*onEvent)(Event_t event);

} thread_t;


typedef enum
{
  THREAD_ID_INFO,
  THREAD_ID_CLI,
  THREAD_ID_MAX
} ThreadId_t;


#endif /* SRC_AP_AP_DEF_H_ */
