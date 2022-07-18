/**
  ******************************************************************************
  * @file    bsp_can.h
  * @author  gxf
  * @version V1.0.0
  * @date    2021.11.16
  * @brief   小远机器人大巡逻车CAN的底层文件
 ==============================================================================
  */
 
#ifndef __BSP_CAN_H
#define __BSP_CAN_H
#include "stm32f10x.h"
  

void bsp_can1_init(u16 baudrate);
u8 can1_send_msg(u8* msg,u8 len);
#endif