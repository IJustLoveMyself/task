/**
  ******************************************************************************
  * @file    bsp_gpio.c
  * @author  gxf
  * @version V1.0.0
  * @date    2021.11.16
  * @brief   小远机器人大巡逻车GPIO的底层文件
 ==============================================================================
  */
 
/**
 * +++++++++++++++++++++++++++++++++++++++++++++++
 * 传感器输入引脚      ：PA4、PA5
 * 江云光电控制引脚    ：PB14、PB15、PA8
 * 充电检测输入引脚    ：PA0、PA1
 * 防碰撞传感器输入    ：PB6、PB7
 * 以太网转串口控制    ：PA11
 * ++++++++++++++++++++++++++++++++++++++++++++++++
 * */

#include "bsp_gpio.h"

/**
 * [bsp_gpiio_init description]
 * @author Gxf
 * @dateTime 2021-11-16
 * @brief    [description] GPIO初始化
 * @note     [description]
 */
 void bsp_gpio_init(void)
 {
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE); 
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE); 

  GPIO_InitStructure.GPIO_Pin    = GPIO_Pin_0 | GPIO_Pin_1| GPIO_Pin_4 | GPIO_Pin_5 ; 
  GPIO_InitStructure.GPIO_Mode   = GPIO_Mode_IN_FLOATING;       
  GPIO_InitStructure.GPIO_Speed  = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure); 
 

  GPIO_InitStructure.GPIO_Pin    = GPIO_Pin_8 | GPIO_Pin_11 ; 
  GPIO_InitStructure.GPIO_Mode   = GPIO_Mode_Out_PP;       
  GPIO_InitStructure.GPIO_Speed  = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_SetBits(GPIOA,GPIO_Pin_11); 

  GPIO_InitStructure.GPIO_Pin    = GPIO_Pin_6 | GPIO_Pin_7  ; 
  GPIO_InitStructure.GPIO_Mode   = GPIO_Mode_IPU;       
  GPIO_InitStructure.GPIO_Speed  = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);  

	
  GPIO_InitStructure.GPIO_Pin    = GPIO_Pin_14 |GPIO_Pin_15; 
  GPIO_InitStructure.GPIO_Mode   = GPIO_Mode_Out_PP;       
  GPIO_InitStructure.GPIO_Speed  = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
 }