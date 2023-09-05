#ifndef __CAN_H
#define __CAN_H
#include "main.h"

uint8_t CAN1_Init(uint8_t tsjw, uint8_t tbs2, uint8_t tbs1, uint16_t brp, uint8_t mode);

uint8_t CAN1_Send_Msg(uint8_t *msg, uint8_t len, uint32_t stdid); // 发送数据
uint8_t CAN1_Receive_Msg(uint8_t *buf);                           // 接收数据

#endif
