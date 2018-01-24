/*
 * Task3.c
 *
 *  Created on: Aug 4, 2017
 *      Author: alvin
 */
#include "Test.h"

void Task3(void const *argument);

void delays(uint32_t const num){
    uint32_t i, j;
    for(i=0;i<num;i++)
        for( j=0;j<num;j++)
            asm volatile("nop");
}

int Init_Task3 (void)
{
	xTaskCreate(Task3, "Task3", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL);
	PRINTF("Pass Task3 Creation\n");
}


void Task3(void const *argument)
{
int flag=0;
uint32_t tamp;
TickType_t freq=20;
	/*
	 * Semaphore test 2
	 */
	if(SemaphoreTest2)
	{
		uint32_t lasttime=0;
		char buffer[30];
		while(1)
		{
			freq+=lasttime;
			PRINTF("========= Task 3 Jalan =======\n");
			if(Semaphore_take(&sem1,1000))
			{
				//PRINTF("Task 3 take semaphore %d\n",getTick());
				PRINTF("task 3 take semaphore %d\n",getTick());
				//USART_puts(buffer);
			}
			else
			{
				//PRINTF("Task 3 failed to take semaphore %d\n",getTick());
				PRINTF("task 3 failed to take semaphore %d\n",getTick());
				//USART_puts(buffer);
			}
			lasttime=freq;
			if(Difpriority)
				vTaskDelay(freq);
			else
				taskYIELD(); //osThreadYield();
		}
	}

	/*
	 * Malloc Test
	 */
	while(MallocTest2)
	{
	  if(flag==0)
	  {
		  void* temp[30];
		  temp[0]=MallocCount(128,NONE,NONE);

		  temp[1]=MallocCount(2048,NONE,NONE);

		  temp[2]=MallocCount(256,NONE,NONE);

		  temp[3]=MallocCount(16,NONE,NONE);

		  FreeCount(temp[1],2048);

		  temp[4]=MallocCount(32,NONE,NONE);

		  FreeCount(temp[0],128);

		  FreeCount(temp[2],256);

		  temp[5]=MallocCount(64,NONE,NONE);

		  temp[6]=MallocCount(2048,NONE,NONE);

		  FreeCount(temp[5],64);

		  temp[7]=MallocCount(1024,NONE,NONE);

		  FreeCount(temp[6],2048);

		  temp[8]=MallocCount(512,NONE,NONE);

		  FreeCount(temp[3],16);

		  temp[9]=MallocCount(8,NONE,NONE);

		  FreeCount(temp[8],512);

		  temp[10]=MallocCount(8,NONE,NONE);

		  FreeCount(temp[7],1024);

		  FreeCount(temp[9],8);

		  temp[11]=MallocCount(512,NONE,NONE);

		  temp[12]=MallocCount(8,NONE,NONE);

		  FreeCount(temp[11],512);

		  temp[13]=MallocCount(256,NONE,NONE);

		  FreeCount(temp[10],8);

		  temp[14]=MallocCount(32,NONE,NONE);

		  temp[15]=MallocCount(32,NONE,NONE);

		  FreeCount(temp[4],32);

		  FreeCount(temp[13],256);

		  FreeCount(temp[15],32);

		  temp[16]=MallocCount(256,NONE,NONE);

		  temp[17]=MallocCount(8,NONE,NONE);

		  temp[18]=MallocCount(32,NONE,NONE);

		  temp[19]=MallocCount(16,NONE,NONE);

		  temp[20]=MallocCount(8,NONE,NONE);

		  FreeCount(temp[12],8);

		  temp[21]=MallocCount(4096,NONE,NONE);

		  FreeCount(temp[20],8);

		  temp[22]=MallocCount(512,NONE,NONE);

		  FreeCount(temp[17],8);

		  temp[23]=MallocCount(128,NONE,NONE);

		  FreeCount(temp[19],16);

		  temp[24]=MallocCount(1024,NONE,NONE);

		  temp[25]=MallocCount(16,NONE,NONE);

		  FreeCount(temp[23],128);

		  temp[26]=MallocCount(1024,NONE,NONE);

		  temp[27]=MallocCount(512,NONE,NONE);

		  FreeCount(temp[21],4096);

		  FreeCount(temp[26],1024);

		  temp[28]=MallocCount(256,NONE,NONE);

		  temp[29]=MallocCount(1024,NONE,NONE);

		  FreeCount(temp[18],32);
		  flag=1;
	  }
	}
	/*
	 * Queue Test1
	 */
	if(QueueTest1)
	{
		uint32_t tampung=0;
		queue_get(&queue1,&tampung,300);
		PRINTF("isi data %d\n",tampung);
		queue_get(&queue1,&tampung,300);
		PRINTF("isi data %d\n",tampung);
		while(1)
		 	taskYIELD(); //osThreadYield();
		queue_get(&queue1,&tampung,300);
		PRINTF("isi data %d\n",tampung);
		queue_get(&queue1,&tampung,300);
		PRINTF("isi data %d\n",tampung);
		  taskYIELD(); //osThreadYield();
		queue_get(&queue1,&tampung,300);
		PRINTF("isi data %d\n",tampung);
		queue_get(&queue1,&tampung,300);
		PRINTF("isi data %d\n",tampung);
		queue_get(&queue1,&tampung,300);
		PRINTF("isi data %d\n",tampung);
		queue_get(&queue1,&tampung,300);
		PRINTF("isi data %d\n",tampung);
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
		char buffer[30];
		int data=3003;
		TickType_t lastwake=xTaskGetTickCount();
		TickType_t freq=20;
		while(data<20000)
		{
			freq+=lasttime;
			if(data<=20000)
			{
				//PRINTF("==Task3 Send %d Tick %d\n",data,getTick());
				PRINTF("Task3 Send %d Tick %d\n",data,getTick());
				//USART_puts(buffer);
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
	if(0)
	{
		uint32_t tampung=0;
		xQueueReceive(queue,&tampung,1000);
		PRINTF("isi data %d\n",tampung);
		xQueueReceive(queue,&tampung,1000);
		PRINTF("isi data %d\n",tampung);
		xQueueReceive(queue,&tampung,1000);
		PRINTF("isi data %d\n",tampung);
		xQueueReceive(queue,&tampung,1000);
		PRINTF("isi data %d\n",tampung);
		xQueueReceive(queue,&tampung,1000);
		PRINTF("isi data %d\n",tampung);
		xQueueReceive(queue,&tampung,1000);
		PRINTF("isi data %d\n",tampung);
		xQueueReceive(queue,&tampung,1000);
		PRINTF("isi data %d\n",tampung);
		xQueueReceive(&queue,&tampung,1000);
		PRINTF("isi data %d\n",tampung);
		xQueueReceive(queue,&tampung,1000);
		PRINTF("isi data %d\n",tampung);
		xQueueReceive(queue,&tampung,1000);
		PRINTF("isi data %d\n",tampung);
		if(Difpriority)
			vTaskDelay(freq);
		else
			  taskYIELD(); //osThreadYield();
	}

	while(Thread_3_test)
	{
		PRINTF("========= Task 3 Jalan =======\n");
		delays(1000);
		if(Difpriority){
			PRINTF("========= Task 3 Div =======\n");
			vTaskDelay(freq);
		}
		else{
			PRINTF("========= Task 3 non - Div =======\n");
			taskYIELD(); //osThreadYield();
		}
	}

}



