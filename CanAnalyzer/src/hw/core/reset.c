/*
 * reset.c
 *
 *  Created on: 2021. 11. 21.
 *      Author: baram
 */




#include "reset.h"
#include "rtc.h"


static uint8_t reset_status = 0x00;
static uint8_t reset_bits   = 0x00;





bool resetInit(void)
{
  if (reset_bits & (1<<_DEF_RESET_WDG))
  {
    reset_status = _DEF_RESET_WDG;
  }
  else if (reset_bits & (1<<_DEF_RESET_SOFT))
  {
    reset_status = _DEF_RESET_SOFT;
  }
  else if (reset_bits & (1<<_DEF_RESET_POWER))
  {
    reset_status = _DEF_RESET_POWER;
  }
  else
  {
    reset_status = _DEF_RESET_PIN;
  }

  return true;
}

void resetLog(void)
{
  if (reset_bits & (1<<_DEF_RESET_POWER))
  {
    logPrintf("ResetFrom \t\t: Power\r\n");
  }
  if (reset_bits & (1<<_DEF_RESET_PIN))
  {
    logPrintf("ResetFrom \t\t: Pin\r\n");
  }
  if (reset_bits & (1<<_DEF_RESET_WDG))
  {
    logPrintf("ResetFrom \t\t: Wdg\r\n");
  }
  if (reset_bits & (1<<_DEF_RESET_SOFT))
  {
    logPrintf("ResetFrom \t\t: Soft\r\n");
  }
}

void resetRunSoftReset(void)
{
  HAL_NVIC_SystemReset();
}

void resetSetBootMode(uint8_t boot_mode)
{
  rtcWriteBackupData(_HW_DEF_RTC_BOOT_MODE, boot_mode);
}

uint8_t resetGetBootMode(void)
{
  return rtcReadBackupData(_HW_DEF_RTC_BOOT_MODE);
}

void resetToRunBoot(void)
{
  uint32_t reg;

  reg = resetGetBootMode();

  reg |= RESET_MODE_RUN_BOOT;
  resetSetBootMode(reg);
  NVIC_SystemReset();
}

void resetToBoot(uint32_t timeout)
{
  resetToRunBoot();
}

void resetClearFlag(void)
{
  __HAL_RCC_CLEAR_RESET_FLAGS();
}

uint8_t resetGetStatus(void)
{
  return reset_status;
}

uint8_t resetGetBits(void)
{
  return reset_bits;
}

void resetSetBits(uint8_t data)
{
  reset_bits = data;
}

