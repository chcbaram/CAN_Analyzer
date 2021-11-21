/*
 * boot.h
 *
 *  Created on: 2021. 11. 21.
 *      Author: baram
 */

#ifndef SRC_AP_BOOT_BOOT_H_
#define SRC_AP_BOOT_BOOT_H_


#include "ap_def.h"



void bootInit(void);
void bootProcessCmd(cmd_t *p_cmd);
void bootJumpToFw(void);
bool bootVerifyFw(void);
bool bootVerifyCrc(void);



#endif /* SRC_AP_BOOT_BOOT_H_ */
