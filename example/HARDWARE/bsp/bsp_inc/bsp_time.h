/**
  ******************************************************************************
  * @file    bsp_time.h
  * @author  gxf
  * @version V1.0.0
  * @date    2021.11.16
  * @brief   小远机器人大巡逻车定时器的底层文件
 ==============================================================================
  */

#ifndef __BSP_TIME_H
#define __BSP_TIME_H
#include "stm32f10x.h"
  
void bsp_time2_init(u16 arr,u16 psc);
void bsp_time3_init(u16 arr,u16 psc);
 
#endif



