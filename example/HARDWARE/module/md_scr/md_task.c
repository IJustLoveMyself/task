/**
  ******************************************************************************
  * @file    md_task.c
  * @author  gxf
  * @version V1.0.0
  * @date    2021.09.06
  * @brief   小远机器人任务控制
 ==============================================================================
 */
#include "md_task.h"
#include <stdio.h>
#include <stdlib.h>
static taskHandle_t *taskList;
static void Idletask(void);
static u8 scheduler_flag = 0; /* 开始调度标志位，标志位置1，表示可以开始调度了 */
/**
 * [task_list_creat description]
 * @author Gxf
 * @dateTime 2021-09-29
 * @brief    [description] 创建一个存放任务块的双向列表
 * @note     [description] head节点的任务状态初始化为Stop,表示这个节点的任务不参与轮询
 * @return   [description] 返回列表的头
 */
static taskHandle_t *task_list_create(void)
{
  taskHandle_t *head;
  head = (taskHandle_t *)malloc(sizeof(taskHandle_t));
  head->state = Stop;
  head->prev  = NULL;
  head->next  = NULL;
  head->func  = &Idletask;
  head->id    = 0;
  return head;
}

/**
 * [Idletask description]
 * @author Gxf
 * @dateTime 2021-09-29
 * @brief    [description] 空闲任务
 * @note     [description]
 */
static void Idletask(void)
{
  /* 可以用来在任务轮询的时候打印一些数据 */
  /* 暂时为空 */
}

/**
 * [task_add description]
 * @author Gxf
 * @dateTime 2021-09-29
 * @brief    [description]  任务添加函数，将函数添加到列表的头节点后
 * @note     [description]  任务的状态设置为Run，表示添加后先运行一遍
 * @param    func          [description] 要添加的函数
 * @param    delay         [description] 任务延时
 * @param    id            [description] 任务的id
 * @param    head          [description] 任务列表头结点
 */
static void task_add(void(*func)(),u32 delay,u8 id,taskHandle_t *head)
{
  taskHandle_t *task;
  task = (taskHandle_t *)malloc(sizeof(taskHandle_t));
  task->func      = func;
  task->timedelay = delay;
  task->timecnt   = delay;
  task->state     = Run;
  task->id        = id;
  if(head->next == NULL)
  {
  	/* 当任务列表只有一个头节点的时候 */  
    task->prev = head;
    task->next = head->next;
    head->next = task;
  }else{
  	/* 当任务列表有多个节点的时候 */
    task->prev       = head;
    task->next       = head->next;
    head->next->prev = task;
    head->next       = task;    
  }	
}

/**
 * [task_change_state description]
 * @author Gxf
 * @dateTime 2021-09-29
 * @brief    [description]
 * @note     [description]
 * @param    id            [description] 要修改状态的任务id
 * @param    state         [description] 状态值
 * @param    head          [description] 任务列表头结点
 */
static void task_change_state2(u8 id,task_s state,taskHandle_t *head)
{
  taskHandle_t *node = head;
  taskHandle_t *node_t = NULL;
  do
  {
    if(node->id == id)
    {
      node->state = state;
      break;
    }
    node_t = node->next;
    node   = node_t;     
  } while (node != NULL);  
}

/**
 * [task_polling description]
 * @author Gxf
 * @dateTime 2021-09-29
 * @brief    [description] 任务轮询函数
 * @note     [description]
 * @param    head          [description] 任务列表的头节点
 */
static void task_polling(taskHandle_t *head)
{
  taskHandle_t *node = head;
  taskHandle_t *node_t = NULL;
  do
  {
    if(node->state == Run)
    {
      node->func();
      node->state = Suppend;
    }
    node_t = node->next;
    node   = node_t;
  } while (node != NULL);  
}
/**
 * [task_timeslice_tick description]
 * @author Gxf
 * @dateTime 2021-09-29
 * @brief    [description] 时间片轮询函数
 * @note     [description]
 * @param    head          [description] 任务列表的头节点
 */
static void task_timeslice_tick(taskHandle_t *head)
{
  taskHandle_t *node = head;
  taskHandle_t *node_t = NULL;
  do
  {
    if(node->state == Suppend)
    {
      if(node->timecnt != 0)
      {
        node->timecnt--;
        if(node->timecnt == 0)
        {
          node->state = Run;
          node->timecnt = node->timedelay;
        }
      }
    }
		node_t = node->next;
    node   = node_t;
  } while (node != NULL);
}

/**
 * [task_init description]
 * @author Gxf
 * @dateTime 2021-09-30
 * @brief    [description] 任务初始化
 * @note     [description]
 */
void task_init(void)
{
  /* 创建一个任务列表 */
  taskList = task_list_create();
}

/**
 * [task_start description]
 * @author Gxf
 * @dateTime 2021-09-30
 * @brief    [description] 启动任务轮询
 * @note     [description]
 */
void task_start(void)
{
  /* 任务已经准备完毕，可以开始进行调度 */
  scheduler_flag = 1;
  task_polling(taskList);
}

/**
 * [task_scheduler description]
 * @author Gxf
 * @dateTime 2021-09-30
 * @brief    [description] 任务调度器 这里是通过定时器1实现简单的调度轮询工作
 * @note     [description]
 */
void task_scheduler(void)
{
  if(scheduler_flag == 1)
  {
    task_timeslice_tick(taskList);
  }

}

/**
 * [task_change_state description]
 * @author Gxf
 * @dateTime 2021-09-30
 * @brief    [description]
 * @note     [description]
 * @param    id            [description] 要修改状态的任务ID
 * @param    state         [description] 要修改的状态
 */
void task_change_state(u8 id,task_s state)
{
  task_change_state2(id,state,taskList);
}

/**
 * [task_create description]
 * @author Gxf
 * @dateTime 2021-09-30
 * @brief    [description]  任务添加函数，将函数添加到列表
 * @param    func          [description] 要添加的函数
 * @param    delay         [description] 任务延时
 * @param    id            [description] 任务的id
 */
void task_create(void(*func)(),u32 delay,u8 id)
{
  task_add(func,delay,id,taskList);
}


/**
 * [jiangyun_timer_isr description]
 * @author Gxf
 * @dateTime 2021-11-16
 * @brief    [description] 用于任务轮询的定时器中断
 * @note     [description]
 */
void task_timer_isr(void)
{
  if (TIM_GetITStatus(TASK_TIMER, TIM_IT_Update) != RESET)  //判断中断是否发生
  {   
    task_scheduler();  
    TIM_ClearITPendingBit(TASK_TIMER, TIM_IT_Update  );  //清除中断标志位
  }
}