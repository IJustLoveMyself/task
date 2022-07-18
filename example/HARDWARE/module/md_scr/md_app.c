/**
  ******************************************************************************
  * @file    md_app.c
  * @author  gxf
  * @version V1.0.0
  * @date    2021.11.16
  * @brief   小远机器人大巡逻车海泰电机的底层文件
 ==============================================================================
  */
 
#include "bsp_time.h"
#include "bsp_uart.h"
#include "bsp_gpio.h"
#include "bsp_can.h"
#include "bsp_systick.h"
#include "bsp_sys.h"
#include "md_app.h"
#include "md_task.h"


static void motor_init(void);
static void app_init(void);
static void test(void);
/**
 * [bsp_init description]
 * @author Gxf
 * @dateTime 2021-11-16
 * @brief    [description] 底层驱动初始化
 * @note     [description]
 */
void bsp_init(void)
{
	SystemInit();
  sysTick_init();
  nvic_configuration();
  bsp_can1_init(500);
  bsp_gpio_init();
  bsp_uart1_init(115200);
  bsp_uart2_init(9600);
  bsp_time2_init(10,7200); /* 1ms一次中断 */
}

/**
 * [md_init description]
 * @author Gxf
 * @dateTime 2021-11-16
 * @brief    [description] 模块初始化
 * @note     [description]
 */
void module_init(void)
{
  task_init();
  app_init();
}


/**
 * [app_init description]
 * @author Gxf
 * @dateTime 2021-11-16
 * @brief    [description] 添加轮询任务
 * @note     [description]
 */
static void app_init(void)
{
	
  /* 100ms 调用一次test函数*/
  task_create(test,100,1);
}


static void test(void)
{
  int i;
	i++;
}
