/*
 * esp32.c
 *
 *  Created on: 2021. 11. 14.
 *      Author: baram
 */




#include "esp32.h"
#include "gpio.h"
#include "uart.h"
#include "cli.h"
#include "usb.h"


#ifdef _USE_HW_ESP32


#define ESP_RESET_NONE    0
#define ESP_RESET_BOOT    1
#define ESP_RESET_RUN     2




static bool is_download_mode = false;

static uint8_t req_boot_reset = 0;
static bool req_set_baud   = false;

static uint8_t  uart_down_ch  = _DEF_UART3;
static uint8_t  uart_run_ch   = _DEF_UART4;
static uint8_t  usb_ch        = _DEF_UART2;
static uint32_t uart_baud_req = 115200;


#ifdef _USE_HW_CLI
static void cliEsp32(cli_args_t *args);
#endif



bool esp32Init(void)
{
  esp32Reset(false);


#ifdef _USE_HW_CLI
  cliAdd("esp32", cliEsp32);
#endif
  return true;
}

void esp32Reset(bool boot)
{
  if (boot == true)
  {
    uartFlush(uart_down_ch);
    uartFlush(uart_run_ch);
    uartFlush(usb_ch);

    gpioPinWrite(_PIN_GPIO_ESP_RST, _DEF_HIGH);
    gpioPinWrite(_PIN_GPIO_ESP_BOOT,_DEF_LOW);
    delay(1);
    gpioPinWrite(_PIN_GPIO_ESP_RST, _DEF_LOW);
    delay(1);
    gpioPinWrite(_PIN_GPIO_ESP_RST, _DEF_HIGH);
  }
  else
  {
    uartFlush(uart_down_ch);
    uartFlush(uart_run_ch);
    uartFlush(usb_ch);

    gpioPinWrite(_PIN_GPIO_ESP_RST, _DEF_HIGH);
    gpioPinWrite(_PIN_GPIO_ESP_BOOT,_DEF_HIGH);
    delay(1);
    gpioPinWrite(_PIN_GPIO_ESP_RST, _DEF_LOW);
    delay(1);
    gpioPinWrite(_PIN_GPIO_ESP_RST, _DEF_HIGH);
  }
}

void esp32SetBaud(uint32_t baud)
{
  req_set_baud = true;
  uart_baud_req = baud;
}


void esp32RequestBoot(uint8_t param)
{

  if (param == 0x01)  // 0 bit:DTR, 1 bit:RTS
  {
    is_download_mode = true;
    req_boot_reset = ESP_RESET_BOOT;
  }

  if (param == 0x02) // 0 bit:DTR, 1 bit:RTS
  {
    req_boot_reset = ESP_RESET_RUN;
  }
}

void esp32DownloadMode(bool enable)
{
  is_download_mode = enable;
}

bool esp32IsDownloadMode(void)
{
  return is_download_mode;
}

void esp32Update(void)
{
  uint8_t data;


  if (req_boot_reset == ESP_RESET_BOOT)
  {
    req_boot_reset = ESP_RESET_NONE;
    esp32Reset(true);
  }
  if (req_boot_reset == ESP_RESET_RUN)
  {
    req_boot_reset = ESP_RESET_NONE;

    if (is_download_mode == true)
    {
      is_download_mode = false;
      esp32Reset(false);
    }
  }

  if (req_set_baud == true)
  {
    req_set_baud = false;

    if (is_download_mode == true)
    {
      if (uartGetBaud(uart_down_ch) != uart_baud_req)
      {
        uartOpen(uart_down_ch, uart_baud_req);
      }
    }
    else
    {
      if (usbGetType() == USB_CON_ESP)
      {
        if (uartGetBaud(uart_run_ch) != uart_baud_req)
        {
          uartOpen(uart_run_ch, uart_baud_req);
        }
      }
    }
  }


  if (is_download_mode == true)
  {
    while(uartAvailable(uart_down_ch) > 0)
    {
      data = uartRead(uart_down_ch);
      uartWrite(usb_ch, &data, 1);
    }

    while(uartAvailable(usb_ch) > 0)
    {
      data = uartRead(usb_ch);
      uartWrite(uart_down_ch, &data, 1);
    }
  }
  else
  {
    if (usbGetType() == USB_CON_ESP)
    {
      while(uartAvailable(uart_run_ch) > 0)
      {
        data = uartRead(uart_run_ch);
        uartWrite(usb_ch, &data, 1);
      }

      while(uartAvailable(usb_ch) > 0)
      {
        data = uartRead(usb_ch);
        uartWrite(uart_run_ch, &data, 1);
      }
    }
  }
}


#ifdef _USE_HW_CLI
void cliEsp32(cli_args_t *args)
{
  bool ret = false;


  if (args->argc == 1 && args->isStr(0, "boot") == true)
  {
    esp32Reset(true);

    while(1)
    {
      uint8_t data;

      while(uartAvailable(uart_down_ch) > 0)
      {
        data = uartRead(uart_down_ch);
        logPrintf("%c", data, data);
      }
    }

    ret = true;
  }

  if (args->argc == 1 && args->isStr(0, "run") == true)
  {
    esp32Reset(false);
    ret = true;
  }

  if (args->argc == 1 && args->isStr(0, "show") == true)
  {
    while(cliKeepLoop())
    {
      uint8_t data;

      while(uartAvailable(uart_down_ch) > 0)
      {
        data = uartRead(uart_down_ch);
        logPrintf("%c", data);
      }
    }

    ret = true;
  }

  if (args->argc == 1 && args->isStr(0, "update") == true)
  {
    while(cliKeepLoop())
    {
      esp32Update();
    }

    ret = true;
  }

  if (args->argc == 1 && args->isStr(0, "bypass") == true)
  {
    uint8_t data;

    while(cliKeepLoop())
    {
      while(uartAvailable(uart_down_ch) > 0)
      {
        data = uartRead(uart_down_ch);
        uartWrite(usb_ch, &data, 1);
      }

      while(uartAvailable(usb_ch) > 0)
      {
        data = uartRead(usb_ch);
        uartWrite(uart_down_ch, &data, 1);
      }
    }

    ret = true;
  }

  if (ret != true)
  {
    cliPrintf("esp32 boot\n");
    cliPrintf("esp32 run\n");
    cliPrintf("esp32 show\n");
    cliPrintf("esp32 update\n");
    cliPrintf("esp32 bypass\n");
  }
}
#endif


#endif
