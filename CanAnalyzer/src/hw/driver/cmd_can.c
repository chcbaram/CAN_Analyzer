/*
 * cmd_can.c
 *
 *  Created on: 2021. 12. 10.
 *      Author: hancheol
 */




#include "cmd_can.h"
#include "cli.h"


#ifdef _USE_HW_CMD_CAN



#define CMD_CAN_STX                 0xF0
#define CMD_CAN_ETX                 0xFE


typedef enum
{
  CMD_CAN_STATE_WAIT_STX,
  CMD_CAN_STATE_WAIT_TYPE,
  CMD_CAN_STATE_WAIT_CMD,
  CMD_CAN_STATE_WAIT_LENGTH_L,
  CMD_CAN_STATE_WAIT_LENGTH_H,
  CMD_CAN_STATE_WAIT_ADDR_3,
  CMD_CAN_STATE_WAIT_ADDR_2,
  CMD_CAN_STATE_WAIT_ADDR_1,
  CMD_CAN_STATE_WAIT_ADDR_0,
  CMD_CAN_STATE_WAIT_DATA,
  CMD_CAN_STATE_WAIT_CRC,
  CMD_CAN_STATE_WAIT_ETX
} CmdCanState_t;





void cmdCanInit(cmd_can_t *p_cmd, cmd_can_driver_t *p_driver)
{
  p_cmd->is_open  = false;
  p_cmd->p_driver = p_driver;
  p_cmd->state    = CMD_CAN_STATE_WAIT_STX;

  p_cmd->rx_packet.data = &p_cmd->rx_packet.buffer[CMD_CAN_STATE_WAIT_DATA];
  p_cmd->tx_packet.data = &p_cmd->tx_packet.buffer[CMD_CAN_STATE_WAIT_DATA];

  p_cmd->is_init = true;
}

bool cmdCanOpen(cmd_can_t *p_cmd)
{
  p_cmd->is_open = true;
  p_cmd->state = CMD_CAN_STATE_WAIT_STX;
  p_cmd->pre_time = millis();

  return true;
}

bool cmdCanClose(cmd_can_t *p_cmd)
{
  p_cmd->is_open = false;
  return true;
}

bool cmdCanIsBusy(cmd_can_t *p_cmd)
{
  return p_cmd->p_driver->available();
}

bool cmdCanReceivePacket(cmd_can_t *p_cmd)
{
  bool ret = false;
  uint8_t rx_data;
  cmd_can_driver_t *p_driver = p_cmd->p_driver;


  if (p_cmd->is_open != true) return false;


  while(ret != true && p_driver->available() > 0)
  {
    rx_data = p_driver->read();

    if (millis()-p_cmd->pre_time >= 100)
    {
      p_cmd->state = CMD_CAN_STATE_WAIT_STX;
    }
    p_cmd->pre_time = millis();

    switch(p_cmd->state)
    {
      case CMD_CAN_STATE_WAIT_STX:
        if (rx_data == CMD_CAN_STX)
        {
          p_cmd->state = CMD_CAN_STATE_WAIT_TYPE;
          p_cmd->rx_packet.crc = 0 ^ rx_data;
        }
        break;

      case CMD_CAN_STATE_WAIT_TYPE:
        p_cmd->rx_packet.type = rx_data;
        p_cmd->rx_packet.crc ^= rx_data;
        p_cmd->state = CMD_CAN_STATE_WAIT_CMD;
        break;

      case CMD_CAN_STATE_WAIT_CMD:
        p_cmd->rx_packet.cmd  = rx_data;
        p_cmd->rx_packet.crc ^= rx_data;
        p_cmd->state = CMD_CAN_STATE_WAIT_LENGTH_L;
        break;

      case CMD_CAN_STATE_WAIT_LENGTH_L:
        p_cmd->rx_packet.length = rx_data;
        p_cmd->rx_packet.crc   ^= rx_data;
        p_cmd->state = CMD_CAN_STATE_WAIT_LENGTH_H;
        break;

      case CMD_CAN_STATE_WAIT_LENGTH_H:
        p_cmd->rx_packet.length |= (rx_data << 8);
        p_cmd->rx_packet.crc    ^= rx_data;

        if (p_cmd->rx_packet.length >= 4)
        {
          p_cmd->index = 0;
          p_cmd->rx_packet.length -= 4;
          p_cmd->state = CMD_CAN_STATE_WAIT_ADDR_3;
        }
        else
        {
          p_cmd->state = CMD_CAN_STATE_WAIT_STX;
        }
        break;

      case CMD_CAN_STATE_WAIT_ADDR_3:
        p_cmd->rx_packet.addr = rx_data << 24;
        p_cmd->rx_packet.crc ^= rx_data;
        p_cmd->state = CMD_CAN_STATE_WAIT_ADDR_2;
        break;

      case CMD_CAN_STATE_WAIT_ADDR_2:
        p_cmd->rx_packet.addr |= rx_data << 16;
        p_cmd->rx_packet.crc  ^= rx_data;
        p_cmd->state = CMD_CAN_STATE_WAIT_ADDR_1;
        break;

      case CMD_CAN_STATE_WAIT_ADDR_1:
        p_cmd->rx_packet.addr |= rx_data << 8;
        p_cmd->rx_packet.crc  ^= rx_data;
        p_cmd->state = CMD_CAN_STATE_WAIT_ADDR_0;
        break;

      case CMD_CAN_STATE_WAIT_ADDR_0:
        p_cmd->rx_packet.addr |= rx_data << 0;
        p_cmd->rx_packet.crc  ^= rx_data;

        if (p_cmd->rx_packet.length > 0)
          p_cmd->state = CMD_CAN_STATE_WAIT_DATA;
        else
          p_cmd->state = CMD_CAN_STATE_WAIT_CRC;
        break;

      case CMD_CAN_STATE_WAIT_DATA:
        p_cmd->rx_packet.data[p_cmd->index] = rx_data;
        p_cmd->rx_packet.crc ^= rx_data;
        p_cmd->index++;
        if (p_cmd->index == p_cmd->rx_packet.length)
        {
          p_cmd->state = CMD_CAN_STATE_WAIT_CRC;
        }
        break;

      case CMD_CAN_STATE_WAIT_CRC:
        p_cmd->rx_packet.crc_recv = rx_data;
        p_cmd->state = CMD_CAN_STATE_WAIT_ETX;
        break;

      case CMD_CAN_STATE_WAIT_ETX:
        if (rx_data == CMD_CAN_ETX)
        {
          if (p_cmd->rx_packet.crc == p_cmd->rx_packet.crc_recv)
          {
            ret = true;
          }
        }
        p_cmd->state = CMD_CAN_STATE_WAIT_STX;
        break;
    }
  }

  return ret;
}

bool cmdCanSendCmd(cmd_can_t *p_cmd, CmdCanType_t type, CmdCanCmd_t cmd, uint32_t addr, uint8_t *p_data, uint32_t length)
{
  uint32_t index;
  cmd_can_driver_t *p_driver = p_cmd->p_driver;
  uint32_t pkt_len;
  uint32_t data_len;

  if (p_cmd->is_open != true) return false;
  if (length < 4) return false;

  pkt_len  = length;
  data_len = length - 4;

  index = 0;
  p_cmd->tx_packet.buffer[index++] = CMD_CAN_STX;
  p_cmd->tx_packet.buffer[index++] = type;
  p_cmd->tx_packet.buffer[index++] = cmd;
  p_cmd->tx_packet.buffer[index++] = (pkt_len >> 0) & 0xFF;
  p_cmd->tx_packet.buffer[index++] = (pkt_len >> 8) & 0xFF;
  p_cmd->tx_packet.buffer[index++] = (addr >> 24) & 0xFF;
  p_cmd->tx_packet.buffer[index++] = (addr >> 16) & 0xFF;
  p_cmd->tx_packet.buffer[index++] = (addr >>  8) & 0xFF;
  p_cmd->tx_packet.buffer[index++] = (addr >>  0) & 0xFF;

  for (int i=0; i<data_len; i++)
  {
    p_cmd->tx_packet.buffer[index++] = p_data[i];
  }

  uint8_t crc = 0;
  for (int i=0; i<index; i++)
  {
    crc ^= p_cmd->tx_packet.buffer[i];
  }
  p_cmd->tx_packet.buffer[index++] = crc;
  p_cmd->tx_packet.buffer[index++] = CMD_CAN_ETX;

  p_driver->write(p_cmd->tx_packet.buffer, index);

  return true;
}

bool cmdCanSendCmdPacket(cmd_can_t *p_cmd, cmd_can_packet_t *p_packet)
{
  uint32_t index;
  cmd_can_driver_t *p_driver = p_cmd->p_driver;
  uint32_t pkt_len;
  uint32_t data_len;

  if (p_cmd->is_open != true) return false;

  pkt_len  = p_packet->length + 4;
  data_len = p_packet->length;

  index = 0;
  p_cmd->tx_packet.buffer[index++] = CMD_CAN_STX;
  p_cmd->tx_packet.buffer[index++] = p_packet->type;
  p_cmd->tx_packet.buffer[index++] = p_packet->cmd;
  p_cmd->tx_packet.buffer[index++] = (pkt_len >> 0) & 0xFF;
  p_cmd->tx_packet.buffer[index++] = (pkt_len >> 8) & 0xFF;
  p_cmd->tx_packet.buffer[index++] = (p_packet->addr >> 24) & 0xFF;
  p_cmd->tx_packet.buffer[index++] = (p_packet->addr >> 16) & 0xFF;
  p_cmd->tx_packet.buffer[index++] = (p_packet->addr >>  8) & 0xFF;
  p_cmd->tx_packet.buffer[index++] = (p_packet->addr >>  0) & 0xFF;

  for (int i=0; i<data_len; i++)
  {
    p_cmd->tx_packet.buffer[index++] = p_packet->data[i];
  }

  uint8_t crc = 0;
  for (int i=0; i<index; i++)
  {
    crc ^= p_cmd->tx_packet.buffer[i];
  }
  p_cmd->tx_packet.buffer[index++] = crc;
  p_cmd->tx_packet.buffer[index++] = CMD_CAN_ETX;

  p_driver->write(p_cmd->tx_packet.buffer, index);

  return true;
}

#if 0
void cmdSendResp(cmd_t *p_cmd, uint8_t cmd, uint8_t err_code, uint8_t *p_data, uint32_t length)
{
  uint32_t index;


  index = 0;

  p_cmd->tx_packet.buffer[index++] = CMD_STX;
  p_cmd->tx_packet.buffer[index++] = cmd;
  p_cmd->tx_packet.buffer[index++] = CMD_DIR_S_TO_M;
  p_cmd->tx_packet.buffer[index++] = err_code;
  p_cmd->tx_packet.buffer[index++] = (length >> 0) & 0xFF;
  p_cmd->tx_packet.buffer[index++] = (length >> 8) & 0xFF;

  for (int i=0; i<length; i++)
  {
    p_cmd->tx_packet.buffer[index++] = p_data[i];
  }

  uint8_t check_sum = 0;

  for (int i=0; i<length + 5; i++)
  {
    check_sum ^= p_cmd->tx_packet.buffer[i+1];
  }
  p_cmd->tx_packet.buffer[index++] = check_sum;
  p_cmd->tx_packet.buffer[index++] = CMD_ETX;


  uartWrite(p_cmd->ch, p_cmd->tx_packet.buffer, index);
}

bool cmdSendCmdRxResp(cmd_t *p_cmd, uint8_t cmd, uint8_t *p_data, uint32_t length, uint32_t timeout)
{
  bool  ret = false;
  uint32_t time_pre;


  cmdSendCmd(p_cmd, cmd, p_data, length);

  time_pre = millis();

  while(1)
  {
    if (cmdReceivePacket(p_cmd) == true)
    {
      ret = true;
      break;
    }

    if (millis()-time_pre >= timeout)
    {
      break;
    }
  }

  return ret;
}
#endif


#ifdef _USE_HW_CLI

void cliButton(cli_args_t *args)
{
  bool ret = false;


  if (args->argc == 1 && args->isStr(0, "show"))
  {
    while(cliKeepLoop())
    {
      delay(100);
    }

    ret = true;
  }


  if (ret != true)
  {
    cliPrintf("cmdcan show\n");
  }
}
#endif



#endif
