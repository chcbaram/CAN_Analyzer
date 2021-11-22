/*
 * thread.h
 *
 *  Created on: 2021. 11. 23.
 *      Author: baram
 */

#ifndef SRC_AP_THREAD_THREAD_H_
#define SRC_AP_THREAD_THREAD_H_


#include "ap_def.h"



bool threadInit(void);
bool threadNotify(Event_t event);


#endif /* SRC_AP_THREAD_THREAD_H_ */
