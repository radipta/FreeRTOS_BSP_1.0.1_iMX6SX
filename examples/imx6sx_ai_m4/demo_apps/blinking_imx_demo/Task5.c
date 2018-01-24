/*
 * Task5.c
 *
 *  Created on: Aug 4, 2017
 *      Author: alvin
 */
#include "Test.h"

osThreadId Task5Handle;
void Task5(void const *argument);

int Init_Task5 (void)
{
	osThreadDef(Task5, Task5, osPriorityNormal, 0, 128);
	Task5Handle = osThreadCreate(osThread(Task5), NULL);
	if (Task5Handle)
	  return(0);
	return(1);
}

void Task5(void const *argument)
{
	/*
	 * Queue Test 2 (sender)
	 */

	int data=5005,j=0;
	const TickType_t freq=28;
	if(QueueTest2)
	{
		trace_printf("========= Task 5 Jalan =======\n");
		TickType_t lastwake=xTaskGetTickCount();
		const TickType_t freq=20;
		while(1)
		{
			if(data<=20000)
			{
				trace_printf("==Task5 Send %d Tick %d\n",data,getTick());
				queue_send(&queue1,&data,(TickType_t)1000);
				data+=3000;
				delays(300);
			}
			if(Difpriority)
				vTaskDelay(freq);
			else
				osThreadYield();
		}
	}
	/*
	 * semaphore test
	 */
	while(1)
	{
		trace_printf("========= Task 5 Jalan =======\n");
		if(Semaphore_take(&sem1,1000))
		{
			trace_printf("Task 5 take semaphore %d\n",getTick());
		}
		else
		{
			trace_printf("Task 5 failed to take semaphore %d\n",getTick());
		}
		//j++;
		//osThreadYield();
		if(Difpriority)
			vTaskDelay(freq);
		else
			osThreadYield();
	}
}



