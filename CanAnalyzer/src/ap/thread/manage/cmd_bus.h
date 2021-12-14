/*
 * cmd_bus.h
 *
 *  Created on: 2021. 12. 10.
 *      Author: baram
 */

#ifndef SRC_AP_THREAD_CMD_BUS_H_
#define SRC_AP_THREAD_CMD_BUS_H_


#include "ap_def.h"

namespace ap
{
  
typedef struct
{
  uint32_t (*available)(void);
  bool     (*read)(cmd_can_packet_t *p_packet);
  bool     (*write)(cmd_can_packet_t *p_packet);
} cmdbus_obj_t;

cmdbus_obj_t *cmdBusObj(void);
bool cmdBusThreadInit(thread_t *p_thread);

}

#endif 