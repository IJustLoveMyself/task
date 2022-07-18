/**
  ******************************************************************************
  * @file    bsp_systick.h
  * @author  gxf
  * @version V1.0.0
  * @date    2021.11.16
  * @brief   小远机器人大巡逻车滴答定时器的底层文件
 ==============================================================================
  */
 
#ifndef __BSP_SYSTICK_H
#define __BSP_SYSTICK_H
#include "stm32f10x.h"
void sysTick_init(void);  
void delay_us(u32 nTime);
void delay_ms(u32 nTime);
void TimingDelay_Decrement(void);
#endif
