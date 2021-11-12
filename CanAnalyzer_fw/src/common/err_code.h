/*
 * err_code.h
 *
 *  Created on: Nov 12, 2021
 *      Author: baram
 */

#ifndef SRC_COMMON_ERR_CODE_H_
#define SRC_COMMON_ERR_CODE_H_


#define OK                            0
#define ERR_MEMORY                    1
#define ERR_FULL                      2
#define ERR_EMPTY                     3
#define ERR_NULL                      4
#define ERR_INVAILD_INDEX             5

#define ERR_INVALID_CMD               100
#define ERR_INVALID_LENGTH            239
#define ERR_FLASH_INVALID_CHECK_SUM   240
#define ERR_FLASH_INVALID_TAG         241
#define ERR_INVALID_FW                242
#define ERR_FLASH_ADDR_ALIGN          243
#define ERR_FLASH_INVALID_ADDR        244
#define ERR_FLASH_ERROR               245
#define ERR_FLASH_BUSY                246
#define ERR_FLASH_ERR_TIMEOUT         247
#define ERR_FLASH_NOT_EMPTY           248
#define ERR_FLASH_WRITE               249
#define ERR_FLASH_READ                250
#define ERR_FLASH_ERASE               251
#define ERR_FLASH_PACKET_SIZE         252
#define ERR_FLASH_SIZE                253
#define ERR_FLASH_CRC                 254



#endif /* SRC_COMMON_ERR_CODE_H_ */
