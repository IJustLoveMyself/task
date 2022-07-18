/**
  ******************************************************************************
  * @file    bsp_uart.c
  * @author  gxf
  * @version V1.0.0
  * @date    2021.11.16
  * @brief   小远机器人大巡逻车串口的底层文件
 ==============================================================================
  */
 
#include "bsp_uart.h"
#include <stdio.h>

/**
 * [bsp_uart1_init description]
 * @author Gxf
 * @dateTime 2021-11-16
 * @brief    [description] 串口1初始化
 * @note     [description]
 * @param    baudrate      [description] 串口的波特率
 */
void bsp_uart1_init(u32 baudrate)
{
  /* GPIO端口设置 */
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
	 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	 
  
  /* USART1_TX   GPIOA.9 */
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9; 			 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;	 
  GPIO_Init(GPIOA, &GPIO_InitStructure);					 
   
  /* USART1_RX	  GPIOA.10初始化 */
  GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_10;						 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	 
  GPIO_Init(GPIOA, &GPIO_InitStructure);								 

  
  /* USART 初始化设置 */
  USART_InitStructure.USART_BaudRate            = baudrate; 
  USART_InitStructure.USART_WordLength          = USART_WordLength_8b; 
  USART_InitStructure.USART_StopBits            = USART_StopBits_1; 
  USART_InitStructure.USART_Parity              = USART_Parity_No; 
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; 
  USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx; 

  USART_Init(USART1, &USART_InitStructure);	 
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); 
  USART_Cmd(USART1, ENABLE);  
}
/**
 * [bsp_uart2_init description]
 * @author Gxf
 * @dateTime 2021-11-16
 * @brief    [description] 串口2初始化
 * @note     [description]
 * @param    baudrate      [description] 串口的波特率
 */
void bsp_uart2_init(u32 baudrate)
{
  /* GPIO端口设置 */
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;

	 
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE); 
  USART_DeInit(USART2); 
  /* USART1_TX   PA.2 */
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure); 
  /* USART1_RX	  PA.3 */
  GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  

  USART_InitStructure.USART_BaudRate            = baudrate;
  USART_InitStructure.USART_WordLength          = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits            = USART_StopBits_1;
  USART_InitStructure.USART_Parity              = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;

  USART_Init(USART2, &USART_InitStructure); 
  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
  USART_Cmd(USART2, ENABLE);  
}

/**
 * [USART1_SendByte description]
 * @author Gxf
 * @dateTime 2021-11-16
 * @brief    [description] 串口1单字节发送
 * @note     [description]
 * @param    data          [description] 发送的数据
 */
void usart1_sendbyte(char data){
  while((USART1->SR&0x40)==0);
    USART1->DR=data;
}

/**
 * [usart1_sendbytes description]
 * @author Gxf
 * @dateTime 2021-11-16
 * @brief    [description] 串口1多字节发送
 * @note     [description]
 * @param    str           [description] 待发送数据的地址
 * @param    len           [description] 待发送数据的长度
 */
void usart1_sendbytes(char *str,u8 len)
{
  u8 i = 0;	
  for (i=0;i<len;i++)
    usart1_sendbyte(*str++);
}

/**
 * [usart2_sendbyte description]
 * @author Gxf
 * @dateTime 2021-11-16
 * @brief    [description] 串口2单字节发送
 * @note     [description]
 * @param    data          [description]
 */
void usart2_sendbyte(char data){
  while((USART2->SR&0x40)==0);
    USART2->DR=data;
}

/**
 * [usart2_sendbytes description]
 * @author Gxf
 * @dateTime 2021-11-16
 * @brief    [description] 串口2多字节发送
 * @note     [description]
 * @param    str           [description] 待发送的数据的地址
 * @param    len           [description] 待发送的数据的长度
 */
void usart2_sendbytes(char *str,u8 len)
{
  u8 i = 0;	
  for (i=0;i<len;i++)
    usart2_sendbyte(*str++);
}

/* 重定向c库函数printf到串口DEBUG_USART，重定向后可使用printf函数 */
int fputc(int ch, FILE *f)
{
  USART_SendData(USART2, (uint8_t)ch);
  while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);
  return ch;
}