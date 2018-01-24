/*
 * Task1.c
 *
 *  Created on: Aug 4, 2017
 *      Author: alvin
 */
#include "Test.h"

void Task1(void const *argument);

void delays(uint32_t const num){
    uint32_t i, j;
    for(i=0;i<num;i++)
        for( j=0;j<num;j++)
            asm volatile("nop");
}

int Init_Task1 (void)
{
	xTaskCreate(Task1, "Task1", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL);
	PRINTF("Pass Task1 Creation\n");
}

void Task1(void const *argument)
{
	uint32_t *tamp2,*tamp3;
	char *tamp1,*tamp;
	int flag=0,flags=0;
	TickType_t freq=80;

	/*
	 * Semaphore test 1
		Menggunakan 1 thread untuk mengambil 3 token dan merelease 3 token
		Disertai validasi
	
	 */
	if(SemaphoreTest1)
	{
		PRINTF("========= Semaphore Test 1 =======\n");
		int i=0;
		//GPIO_ResetBits(GPIOD,GPIO_Pin_13);
		PRINTF("========= Task 1 Jalan =======\n");
		while(i<MaxSem)
		{
			if(Semaphorecount(1000))
			{
				
				//PRINTF("semaphore task 1 take %d\n",getTick());
				PRINTF("semaphore task 1 take token\n");
			}
			else
			{
				//PRINTF("Task 1 failed to take semaphore %d\n",getTick());
				PRINTF("Task 1 failed to take semaphore\n");
			}
			i++;
		}		
		uint32_t Semtoken;
		if(flagsemph==0)
			Semtoken=Semaphore_GetCount(&sem1);
		else
			Semtoken=MaxSem;
		PRINTF("Semaphore token : %d\n",Semtoken);
		if(Semtoken==MaxSem)
		{
			i=0;
			while(i<MaxSem)
			{
				if(Semaphoregivecount(1000))
				{
					  
					//PRINTF("task release semaphore  %d\n",getTick());
					PRINTF("task release semaphore\n");
				}
				else
				{
					//PRINTF("Task failed to release semaphore %d\n",getTick());
					PRINTF("Task failed to release semaphore\n");
				}
				i++;
			}
		}
		  
		if(flagsemph==0)
		{
			Semtoken=Semaphore_GetCount(&sem1);
			PRINTF("Semaphore token after its release %d\n",Semtoken);
			if(Semtoken==0)
			{
				PRINTF("Semaphore 1st Test Success\n");
			}
		}
		else
		{
			if(xQueueGenericReceive( ( QueueHandle_t ) ( sem2 ), ((void *)0), (1000), (( BaseType_t) 0 ) ))
				PRINTF("FreeRTOS Semaphore 1st Test failed\n");
			else
				PRINTF("FreeRTOS Semaphore 1st Test success\n");
		}
	}
	/*
	 * Semaphore Test 2
	 */
	if(SemaphoreTest2)
	{
		uint32_t lasttime=0;
		  
		PRINTF("========= Semaphore Test 2 =======\n");
		while(1)
		{
			freq+=lasttime;
			PRINTF("========= Task 1 Jalan =======\n");
			if(Semaphore_take(&sem1,1000))
			{
				//sprintf(buffer,"task 1 take semaphore %d token %d\n",getTick(),Semaphore_GetCount(&sem1));
				//USART_puts(buffer);
				PRINTF("task 1 take semaphore %d token %d\n",getTick(),Semaphore_GetCount(&sem1));
				PRINTF("task 1 take semaphore %d\n",getTick());
			}
			else
			{
				//sprintf(buffer,"task 1 failed semaphore %d token %d\n",getTick(),Semaphore_GetCount(&sem1));
				//USART_puts(buffer);
				PRINTF("task 1 failed semaphore %d token %d\n",getTick(),Semaphore_GetCount(&sem1));
				PRINTF("Task 1 failed to take semaphore %d\n",getTick());
			}
			if((getTick()>118) && SemaphoreTest2mode)
			{
				if(Semaphore_release(&sem1,1000))
				{
					PRINTF("Release token success %d\n",getTick());
					//sprintf(buffer,"task 1 Release token success %d token %d\n",getTick(),Semaphore_GetCount(&sem1));
					//USART_puts(buffer);
					PRINTF("task 1 Release token success %d token %d\n",getTick(),Semaphore_GetCount(&sem1));
				}
				else
				{
					PRINTF("Release token failed %d\n",getTick());
					//sprintf(buffer,"task 1 Release token failed %d token %d\n",getTick(),Semaphore_GetCount(&sem1));
					//USART_puts(buffer);
					PRINTF("task 1 Release token failed %d token %d\n",getTick(),Semaphore_GetCount(&sem1));
				}
			}
			lasttime=freq;
			if(Difpriority)
				vTaskDelay(freq);
			else
				taskYIELD(); //osThreadYield();
		}
	}
	/*
	 * Semaphore test3
	 */
	int i=0,flagsemp=0;
	  
	while(SemaphoreTest3)
	{
		//PRINTF("========= Task 1 Jalan ======= %d\n",getTick());
		PRINTF("========= Task 1 Jalan =======\n");
		//GPIO_ResetBits(GPIOD,GPIO_Pin_13);
		if(Semaphore_take(&sem1,1000))
		{
			PRINTF("semaphore task 1 take %d\n",getTick());
			//PRINTF(buffer,"semaphore task 1 take %u\n",getTick());
			
	
		}
		else
		{
		
			PRINTF("Task 1 failed to take semaphore %d\n",getTick());
			PRINTF("semaphore task 1 failed to take %u\n",getTick());
		
		}
		delays(50);
		if(Semaphore_release(&sem1,1000))
		{
			PRINTF("semaphore task 1 release %d\n",getTick());
		}
		else
		{
			PRINTF("semaphore task 1 failed release %d\n",getTick());
		}

	  i++;
	  if(Difpriority)
	  	vTaskDelay(freq);
	  else
	  	taskYIELD(); //osThreadYield();
	}
	/*
	 * Queue Test 2
	 */
	if(QueueTest2)
	{
		uint32_t lasttime=0;
		int data=1001;
		while(1)
		{
			freq+=lasttime;
			if(data<=20000)
			{
				PRINTF("==Task1 Send %d Tick %d\n",data,getTick());	
				queue_send(&queue1,&data,(TickType_t)1000);
				data+=3000;
				delays(300);
			}
			lasttime=freq;
			if(Difpriority)
				vTaskDelay(freq);
			else
				taskYIELD(); //osThreadYield();
		}

	}
	/*
	 * Memory Test
	 */
	if(0)
	{
		tamp=Malloc_TLSF(8,NULL,NULL);
		*tamp='a';
		PRINTF("pos1 %p\n",tamp);
		tamp1=MallocCount(20,NULL,NULL);
		*tamp1='b';
		PRINTF("pos2 %p %c\n",tamp1,*tamp);
		tamp2=MallocCount(32,NULL,NULL);
		*tamp2=300;
		PRINTF("pos3 %p %c\n",tamp2,*tamp1);
		tamp3=MallocCount(35,NULL,NULL);
		*tamp3=10000000;
		PRINTF("pos4 %p %d\n",tamp3,*tamp2);
		Free_TLSF(tamp3);
		tamp=MallocCount(4096,NULL,NULL);
		PRINTF("pos5 %p %d",tamp,*tamp3);
		while(1)
		{
			if(Difpriority)
				vTaskDelay(freq);
			else
				taskYIELD(); //osThreadYield();
		}
	}
	/*
	* Thread test
	*/
	while(Thread_1_test)
	{
		PRINTF("========= Task 1 Jalan =======\n");
		delays(1000);
		if(Difpriority){
			PRINTF("========= Task 1 Div =======\n");
			vTaskDelay(freq);
		}
		else{
			PRINTF("========= Task 1 non - Div =======\n");
			taskYIELD(); //osThreadYield();
		}
	}
}
