# task
基于时间片轮询的调度框架

example 中是例程，使用的是stm32F103C8T6

src 中是调度框架的源码

移植说明：

调度是基于时间片来进行轮询的，因此需要使用到定时器。将task_scheduler这个函数放在定时器中断里面。中断时间一般为1ms一次，目前我使用的基本为1ms一次

例程中是使用宏定义的方式映射task_timer_isr到定时器中断

~~~c
#define task_timer_isr TIM2_IRQHandler
#define TASK_TIMER     TIM2
~~~

