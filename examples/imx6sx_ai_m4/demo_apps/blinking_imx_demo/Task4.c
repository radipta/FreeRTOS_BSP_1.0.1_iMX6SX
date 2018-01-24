/*
 * Task4.c
 *
 *  Created on: Aug 4, 2017
 *      Author: alvin
 */
#include "Test.h"

//osThreadId Task4Handle;
void Task4(void const *argument);
int loop=0;

void delays(uint32_t const num){
    uint32_t i, j;
    for(i=0;i<num;i++)
        for( j=0;j<num;j++)
            asm volatile("nop");
}

int Init_Task4 (void)
{
	xTaskCreate(Task4, "Task4", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL);
	PRINTF("Pass Task4 Creation\n");
}

void Task4(void const *argument)
{
	uint32_t tamp=0,j=0;
	uint8_t Buffer[20]="testing";
	TickType_t freq=50;
	/*
	 * Semaphore Test 2
	 */
	if(SemaphoreTest2)
	{
		uint32_t lasttime=0;
		char buffer[30];
			uint32_t semtoken=Semaphore_GetCount(&sem1);
			PRINTF("Semaphore Token %d\n",semtoken);
			while(1)
			{
				j=0;
				PRINTF("========= Task 4 Jalan =======\n");
				freq+=lasttime;
				while(j<2)
				{
					if(Semaphore_release(&sem1,1000))
					{
						PRINTF("Release token success %d\n",getTick());
						//sprintf(buffer,"Release token success %d\n",getTick());
						//USART_puts(buffer);
					}
					else
					{
						PRINTF("Release token failed %d\n",getTick());
						//sprintf(buffer,"Release token failed %d\n",getTick());
						//USART_puts(buffer);
					}
					j++;
				}
				semtoken=Semaphore_GetCount(&sem1);
				if(semtoken==0)
					PRINTF("Semaphore 2nd Test Success");
				lasttime=freq;

				if(Difpriority)
					vTaskDelay(freq);
				else
					taskYIELD(); //osThreadYield();
		//		}
			}
	}
	/*
	 * Queue get 2
	 * (Receiver)
	 */
	char buffer[30];
	int i=0;
	uint32_t lasttime=0;
	while(QueueTest2)
	{
		i=0;
		lasttime+=freq;
		if(tamp>19000)
		{
			i=4;
		}
		while(i<3)
		{
			queue_get(&queue1,&tamp,1000);
			PRINTF("Task4 get %d Tick %d\n",tamp,getTick());
			//sprintf(buffer,"Task4 get %d Tick %d\n",tamp,getTick());
			//USART_puts(buffer);
			//PRINTF("data = %d\n",tamp);
			i++;
		}
		freq=lasttime;
		delays(300);
		if(Difpriority)
			vTaskDelay(freq);
		else
			taskYIELD(); //osThreadYield();
	}

		while(Thread_4_test)
	{
		PRINTF("========= Task 4 Jalan =======\n");
		delays(1000);
		if(Difpriority){
			PRINTF("========= Task 4 Div =======\n");
			vTaskDelay(freq);
		}
		else{
			PRINTF("========= Task 4 non - Div =======\n");
			taskYIELD(); //osThreadYield();
		}
	}
}
