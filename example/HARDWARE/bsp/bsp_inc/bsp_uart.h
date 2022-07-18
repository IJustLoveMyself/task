/**
  ******************************************************************************
  * @file    bsp_uart.h
  * @author  gxf
  * @version V1.0.0
  * @date    2021.11.16
  * @brief   小远机器人大巡逻车串口的底层文件
 ==============================================================================
  */
#ifndef __BSP_UART_H
#define __BSP_UART_H
#include "stm32f10x.h"
  

void bsp_uart1_init(u32 baudrate);
void bsp_uart2_init(u32 baudrate); 
void usart1_sendbyte(char data);
void usart1_sendbytes(char *str,u8 len);
void usart2_sendbyte(char data);
void usart1_sendbytes(char *str,u8 len);
#endif