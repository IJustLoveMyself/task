#include "stm32f10x.h"
#include "misc.h"
#include <stdio.h>
#include <string.h> 
#include <stdlib.h>
#include "md_app.h"
#include "md_task.h"




int main(void)
{ 
	bsp_init();
	module_init();
	while(1)
	{	
    task_start();
 	}
}




