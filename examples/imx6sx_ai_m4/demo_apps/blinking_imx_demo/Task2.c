/*
 * Task2.c
 *
 *  Created on: Aug 4, 2017
 *      Author: alvin
 */
#include "Test.h"

//osThreadId Task2Handle;
void Task2(void const *argument);

void delays(uint32_t const num){
    uint32_t i, j;
    for(i=0;i<num;i++)
        for( j=0;j<num;j++)
            asm volatile("nop");
}

int Init_Task2 (void)
{
	xTaskCreate(Task2, "Task2", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL);
	PRINTF("Pass Task2 Creation\n");
}

void Task2(void const *argument)
{
	uint32_t tampung,data;
	TickType_t freq=40;
	char buffer[30];
	/*
	* Queue Test 2
	*/
	if(QueueTest2)
	{
		uint32_t lasttime=0;
		int data=2002;
		TickType_t lastwake=xTaskGetTickCount();
		while(1)
		{
			freq+=lasttime;
			if(data<=20000)
			{
				//PRINTF("==Task2 Send %d Tick %d\n",data,getTick());
				PRINTF("Task2 Send %d Tick %d\n",data,getTick());
				
				queue_send(&queue1,&data,(TickType_t)1000);
				data+=3000;
				delays(300);
			}
			lasttime=freq;
			if(Difpriority)
				vTaskDelay(freq);
			else
				taskYIELD(); //osThreadYield()
		}
	}
	/*
	 * Queue Test 1
	 */
	// if(QueueTest1)
	// {
	// 	data=1000;
	// 	//queue_send(&queue1,&data,0);
	// 	queuecount(data,0);
	// 	//PRINTF("Queue send %d\n",data);
	// 	data=2000;
	// 	//queue_send(&queue1,&data,0);
	// 	queuecount(data,0);
	// 	//PRINTF("Queue send %d\n",data);
	// 	data=3000;
	// 	//queue_send(&queue1,&data,0);
	// 	queuecount(data,0);
	// 	//PRINTF("Queue send %d\n",data);
	// 	data=4000;
	// 	//queue_send(&queue1,&data,0);
	// 	queuecount(data,0);
	// 	//PRINTF("Queue send %d\n",data);
	// 	data=5000;
	// 	//queue_send(&queue1,&data,0);
	// 	queuecount(data,0);
	// 	//PRINTF("Queue send %d\n",data);
	// 	data=6000;
	// 	//queue_send(&queue1,&data,0);
	// 	queuecount(data,0);
	// 	//PRINTF("Queue send %d\n",data);
	// 	data=7000;
	// 	//queue_send(&queue1,&data,0);
	// 	queuecount(data,0);
	// 	//PRINTF("Queue send %d\n",data);
	// 	data=8000;
	// 	//queue_send(&queue1,&data,0);
	// 	queuecount(data,0);
	// 	//PRINTF("Queue send %d\n",data);
	// 	PRINTF("==========check1=========\n");
	// 	data=9000;
	// 	//queue_send(&queue1,&data,500);
	// 	queuecount(data,500);
	// 	//PRINTF("Queue send %d\n",data);
	// 	PRINTF("==========check2=========\n");
	// 	data=10000;
	// 	//queue_send(&queue1,&data,1000);
	// 	queuecount(data,1000);
	// 	//PRINTF("Queue send %d\n",data);
	// 	PRINTF("==========check3=========\n");
	// 	uint32_t tamp;
	// 	//queue_get(&queue1,&tamp,1000);
	// 	queuegetcount(&tamp,1000);
	// 	//PRINTF("== data = %d\n",tamp);
	// 	//queue_get(&queue1,&tamp,1000);
	// 	queuegetcount(&tamp,1000);
	// 	//PRINTF("== data = %d\n",tamp);
	// 	//queue_get(&queue1,&tamp,1000);
	// 	queuegetcount(&tamp,1000);
	// 	//PRINTF("== data = %d\n",tamp);
	// 	data=11000;
	// 	//queue_send(&queue1,&data,500);
	// 	queuecount(data,500);
	// 	//PRINTF("Queue send %d\n",data);
	// 	PRINTF("==========check4=========\n");
	// 	data=12000;
	// 	//queue_send(&queue1,&data,500);
	// 	queuecount(data,500);
	// 	//PRINTF("Queue send %d\n",data);
	// 	PRINTF("==========check5=========\n");
	// 	//queue_get(&queue1,&tamp,1000);
	// 	queuegetcount(&tamp,1000);
	// 	//PRINTF("== data = %d\n",tamp);
	// 	//queue_get(&queue1,&tamp,1000);
	// 	queuegetcount(&tamp,1000);
	// 	//PRINTF("== data = %d\n",tamp);
	// 	//queue_get(&queue1,&tamp,1000);
	// 	queuegetcount(&tamp,1000);
	// 	//PRINTF("== data = %d\n",tamp);
	// 	//queue_get(&queue1,&tamp,1000);
	// 	queuegetcount(&tamp,1000);
	// 	//PRINTF("== data = %d\n",tamp);
	// 	PRINTF("==========check6=========\n");
	// 	//queue_get(&queue1,&tamp,1000);
	// 	queuegetcount(&tamp,1000);
	// 	//PRINTF("== data = %d\n",tamp);
	// 	//queue_get(&queue1,&tamp,1000);
	// 	queuegetcount(&tamp,1000);
	// 	//PRINTF("== data = %d\n",tamp);
	// 	PRINTF("== data get count %d",queue_getcount(&queue1));
	// 	//queue_get(&queue1,&tamp,1000);
	// 	queuegetcount(&tamp,1000);
	// 	//PRINTF("== data = %d\n",tamp);
	// 	//queue_get(&queue1,&tamp,1000);
	// 	queuegetcount(&tamp,1000);
	// 	//PRINTF("== data = %d\n",tamp);
	// 	PRINTF("== data get count %d",queue_getcount(&queue2));
	// 	queue_destroy(&queue1);
	// 	while(1)
	// 	{
	// 		if(Difpriority)
	// 			vTaskDelay(freq);
	// 		else
	// 			taskYIELD();
	// 			//taskYIELD(); //osThreadYield();	cmsis - os
	// 	}
	// }


	/* new testing below*/

	// if(){
	// 	uint32_t tamp;
	// 	if(Semaphore_GetCount(&sem1)==1){
	// 		queuegetcount(&tamp,0);
	// 		PRINTF("task 2 grab data : %d", tamp);
	// 	}
	// 	else{
	// 		vTaskDelay(freq);
	// 	}

	// }
	if(QueueTest1)
	{
		/*
			Pengujian queue Pertama 	metode 	:	L I F O
			Pengujian queue FLOW RTOS pada platform iMX6SX - UDOO NEO FULL
			pada 1 thread (task2.c)
			a. enqueue 6 data
			b. dequeue 3 data
			c. enqueue 4 data
			d. dequeue 3 data
		
		*/
		uint32_t tamp;
		data=1000;	
		queuecount(data,0);
		data=2000;
		queuecount(data,0);		
		data=3000;		
		queuecount(data,0);		
		data=4000;		
		queuecount(data,0);		
		data=5000;		
		queuecount(data,0);		
		data=6000;		
		queuecount(data,0);	

		queuegetcount(&tamp,0);
		queuegetcount(&tamp,0);
		queuegetcount(&tamp,0);
		
		data=7000;
		queuecount(data,0);
		data=8000;
		queuecount(data,0);
		data=9000;
		queuecount(data,0);
		data=10000;
		queuecount(data,0);

		queuegetcount(&tamp,0);
		queuegetcount(&tamp,0);
		queuegetcount(&tamp,0);
		queuegetcount(&tamp,0);
		queuegetcount(&tamp,0);

		PRINTF("== data get count %d",queue_getcount(&queue1));
		queue_destroy(&queue1);
		while(1)
		{
			if(Difpriority)
				vTaskDelay(freq);
			else
				taskYIELD();
		}
	}



	/*	
	 * Semaphore test 2
	 */
	if(SemaphoreTest2)
	{
		uint32_t lasttime=0;
		while(1)
		{
			freq+=lasttime;
			PRINTF("========= Task 2 Jalan =======\n");
			if(Semaphore_take(&sem1,1000))
			{
				PRINTF("task 2 take semaphore %d token %d\n",getTick(),Semaphore_GetCount(&sem1));			
			}
			else
			{
		
				PRINTF("task 2 failed semaphore %d token %d\n",getTick(),Semaphore_GetCount(&sem1));
				
			}
			if(getTick()==224&&SemaphoreTest2mode)
			{
				if(Semaphore_release(&sem1,1000))
				{
					//PRINTF("Release token success %d\n",getTick());
					PRINTF("Task 2 Release token success %d token %d\n",getTick(),Semaphore_GetCount(&sem1));
					
				}
				else
				{
					//PRINTF("Release token failed %d\n",getTick());
					PRINTF("Task 2 Release token failed %d token %d\n",getTick(),Semaphore_GetCount(&sem1));
					
				}
			}
			lasttime=freq;
			if(Difpriority)
			{
				vTaskDelay(freq);
			}
			else
			{
				taskYIELD(); //osThreadYield();
			}
		}
	}
	/*
	 * Semaphore test3
	 */
	while(SemaphoreTest3)
	{
		PRINTF("========= Task 2 Jalan =======\n");
		//PRINTF("========= Task 2 Jalan ======= %d\n",getTick());
		
		//GPIO_ResetBits(GPIOD,GPIO_Pin_14);
		if(Semaphore_take(&sem1,1000))
		{
			PRINTF("task 2 take semaphore %u\n",getTick());	
		}
		else
		{	
			PRINTF("task 2 failed to take semaphore  %u\n",getTick());	
		}

		delays(50);
		PRINTF("Task 2 yield %d\n",getTick());

		taskYIELD(); //osThreadYield();
		if(Semaphore_release(&sem1,1000))
		{
			PRINTF("semaphore task 2 release %u\n",getTick());
			
		}
		else
		{
			PRINTF("semaphore task 2 release %u\n",getTick());
		}

		if(Difpriority)
			vTaskDelay(freq);
		else
			taskYIELD(); //osThreadYield();
	}
	/*
	 * MAlloc Test 1
	 */
	if(MallocTest1)
	{
		uint32_t *tamp2,*tamp3;
		float *tamp,*cg;
		int *tamp1;
		uint32_t hasil;
		tamp=(float*)MallocCount((size_t)64,NULL,NULL);
		*tamp=12.5;
		PRINTF("Isi memory dengan 12.5\n");
		cg=(float*)MallocCount((size_t)64,NULL,NULL);
		*cg=(float)22.5;
		PRINTF("Isi memory2 dengan 22.5\n");
		hasil=*cg+*tamp;
		PRINTF("Hasil %d\n",hasil);
		tamp1=MallocCount((size_t)32,NULL,NULL);
		*tamp1=43430000;
		tamp2=MallocCount(256,NULL,NULL);
		*tamp2=3000;
		PRINTF("pos8 %d\n",*tamp1);
		Free_TLSF(tamp2);
		tamp3=MallocCount(256,NULL,NULL);
		*tamp3=1000;
		PRINTF("pos9 %d\n",*tamp2);
		tamp=MallocCount(64,NULL,NULL);
		PRINTF("pos10 %d",*tamp3);
		while(1)
		{
			if(Difpriority)
				vTaskDelay(freq);
			else
				taskYIELD(); //osThreadYield();
		}
	}

	while(Thread_2_test)
	{
		PRINTF("========= Task 2 Jalan =======\n");
		delays(1000);
		if(Difpriority){
			PRINTF("========= Task 2 Div =======\n");
			vTaskDelay(freq);
		}
		else{
			PRINTF("========= Task 2 non - Div =======\n");
			taskYIELD(); //osThreadYield();
		}
	}
}



