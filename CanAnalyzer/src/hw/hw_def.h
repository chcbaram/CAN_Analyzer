/*
 * hw_def.h
 *
 *  Created on: Nov 12, 2021
 *      Author: baram
 */

#ifndef SRC_HW_HW_DEF_H_
#define SRC_HW_HW_DEF_H_


#include "def.h"
#include "bsp.h"


#define _DEF_FIRMWATRE_VERSION    "V211213R1"
#define _DEF_BOARD_NAME           "CanAnalyzer"



#define _HW_DEF_RTOS_MEM_SIZE(x)              ((x)/4)


#define _HW_DEF_RTOS_THREAD_PRI_MAIN          osPriorityNormal
#define _HW_DEF_RTOS_THREAD_PRI_CLI           osPriorityNormal
#define _HW_DEF_RTOS_THREAD_PRI_LED           osPriorityNormal
#define _HW_DEF_RTOS_THREAD_PRI_EVENT         osPriorityNormal
#define _HW_DEF_RTOS_THREAD_PRI_INFO          osPriorityNormal
#define _HW_DEF_RTOS_THREAD_PRI_LCD           osPriorityNormal
#define _HW_DEF_RTOS_THREAD_PRI_CAN_BUS       osPriorityNormal
#define _HW_DEF_RTOS_THREAD_PRI_CMD_BUS       osPriorityNormal

#define _HW_DEF_RTOS_THREAD_MEM_MAIN          _HW_DEF_RTOS_MEM_SIZE( 2*1024)
#define _HW_DEF_RTOS_THREAD_MEM_CLI           _HW_DEF_RTOS_MEM_SIZE( 6*1024)
#define _HW_DEF_RTOS_THREAD_MEM_LED           _HW_DEF_RTOS_MEM_SIZE(    256)
#define _HW_DEF_RTOS_THREAD_MEM_EVENT         _HW_DEF_RTOS_MEM_SIZE( 2*1024)
#define _HW_DEF_RTOS_THREAD_MEM_INFO          _HW_DEF_RTOS_MEM_SIZE(    512)
#define _HW_DEF_RTOS_THREAD_MEM_LCD           _HW_DEF_RTOS_MEM_SIZE( 1*1024)
#define _HW_DEF_RTOS_THREAD_MEM_CAN_BUS       _HW_DEF_RTOS_MEM_SIZE( 2*1024)
#define _HW_DEF_RTOS_THREAD_MEM_CMD_BUS       _HW_DEF_RTOS_MEM_SIZE( 1*1024)



#define _USE_HW_ESP32
#define _USE_HW_QSPI
#define _USE_HW_FLASH
#define _USE_HW_RESET
#define _USE_HW_RTOS

#define _USE_HW_LED
#define      HW_LED_MAX_CH          4

#define _USE_HW_UART
#define      HW_UART_MAX_CH         4

#define _USE_HW_BUTTON
#define      HW_BUTTON_MAX_CH       1

#define _USE_HW_GPIO
#define      HW_GPIO_MAX_CH         10

#define _USE_HW_SPI
#define      HW_SPI_MAX_CH          1

#define _USE_HW_CAN
#define      HW_CAN_MAX_CH          2
#define      HW_CAN_MSG_RX_BUF_MAX  16

#define _USE_HW_LOG
#define      HW_LOG_CH              _DEF_UART1
#define      HW_LOG_BOOT_BUF_MAX    1024
#define      HW_LOG_LIST_BUF_MAX    1024

#define _USE_HW_CLI
#define      HW_CLI_CMD_LIST_MAX    16
#define      HW_CLI_CMD_NAME_MAX    16
#define      HW_CLI_LINE_HIS_MAX    4
#define      HW_CLI_LINE_BUF_MAX    64

#define _USE_HW_USB
#define _USE_HW_CDC
#define      HW_USE_CDC             1
#define      HW_USE_MSC             0

#define _USE_HW_RTC
#define      HW_RTC_CFG_DATA_1      RTC_BKP_DR1
#define      HW_RTC_CFG_DATA_2      RTC_BKP_DR2

#define _USE_HW_FS
#define      HW_FS_MAX_SIZE         (8*1024*1024)

#define _USE_HW_LCD     
#define      HW_LCD_LVGL

#define _USE_HW_ST7735
#define      HW_ST7735_MODEL        0
#define      HW_LCD_WIDTH           160
#define      HW_LCD_HEIGHT          80

#define _USE_HW_CMD_CAN
#define      HW_CMD_CAN_MAX_DATA_LENGTH 1024


#define _HW_DEF_RTC_BOOT_RESET      RTC_BKP_DR3
#define _HW_DEF_RTC_BOOT_MODE       RTC_BKP_DR4
#define _HW_DEF_RTC_RESET_SRC       RTC_BKP_DR5


#define _DEF_UART_CLI               _DEF_UART1
#define _DEF_UART_USB               _DEF_UART2
#define _DEF_UART_ESP_BOOT          _DEF_UART3
#define _DEF_UART_ESP_UART          _DEF_UART4

#define _DEF_LED_CAN1               _DEF_LED1
#define _DEF_LED_CAN2               _DEF_LED2
#define _DEF_LED_RUN                _DEF_LED3
#define _DEF_LED_MODE               _DEF_LED4


#define _PIN_GPIO_ESP_RST           2
#define _PIN_GPIO_ESP_BOOT          3

#define _PIN_GPIO_LCD_RST           6
#define _PIN_GPIO_LCD_BLK           7
#define _PIN_GPIO_LCD_CS            8
#define _PIN_GPIO_LCD_DC            9



#endif /* SRC_HW_HW_DEF_H_ */
