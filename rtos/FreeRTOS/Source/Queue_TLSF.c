/*
 * Queue_TLSF.c
 *
 *  Created on: Oct 7, 2016
 *      Author: alvin
 */
#include <stdlib.h>
#include <string.h>
//#include <diag/Trace.h>
#define MPU_WRAPPERS_INCLUDED_FROM_API_FILE

#if ( configUSE_CO_ROUTINES == 1 )
	#include "croutine.h"
#endif

#include "FreeRTOS.h"
#include "task.h"
#include "Queue_TLSF.h"

#undef MPU_WRAPPERS_INCLUDED_FROM_API_FILE /*lint !e961 !e750. */

int queue_create(Queue_Handler* queue_id,size_t length,size_t itemsize,uint32_t mode)
{
	uint32_t* pool;
	size_t total;

	if(length==0||length==0)
		return 0;
	if(itemsize==0||itemsize==0)
		return 0;
	total=itemsize*length;
	PRINTF("total %d\n",total);
	pool=Malloc_TLSF(total,NONE,NONE);
	PRINTF("pool %x\n",pool);
	if(pool==0)
		return 0;

	queue_id->point=pool;
	queue_id->head=queue_id->point;
	queue_id->tail=queue_id->head;
	queue_id->time=0;
	queue_id->count=0;
	queue_id->mode=mode;
	PRINTF("point %x , %x,count %d\n",queue_id->point,queue_id->tail,queue_id->count);

	queue_id->length_queue=length;
	PRINTF("Total Blocks %d\n",queue_id->length_queue);
	queue_id->size_queue=itemsize;
	PRINTF("Size of Block %d\n",queue_id->size_queue);
	//return (void*)queue_id;
	return 1;
}

int queue_send(Queue_Handler* queue_id,void* message,uint32_t timeout)
{
	int check=0;
	uint32_t tick;
	uint32_t total=queue_id->length_queue*queue_id->size_queue;
	void* last;

	if(queue_id->point == NULL || message == NULL)
	{
		return 0;
	}
	vTaskSuspendAll();
	last=((char*)(queue_id->point+total));
	//PRINTF("total %d last %x\n",total,last);
	if(queue_id->count==queue_id->length_queue)
	{
		//PRINTF("----------queue full %d-----------\n",queue_id->count);
		check=1;
	}

	if(check == 0)
	{
		//PRINTF("checking2",total);
		if((queue_id->tail==last)&&(queue_id->head!=queue_id->point))
		{
			queue_id->tail=queue_id->point;
			//PRINTF("check akhir\n");
		}
		( void )memcpy(queue_id->tail,message,queue_id->size_queue);
		queue_id->tail=(Queue_Handler*)((char*)(queue_id->tail+queue_id->size_queue));
		queue_id->count=queue_id->count+1;
		if(listLIST_IS_EMPTY(&(queue_id->TaskWaitQueue))== 0)
		{
			//PRINTF("list tidak kosong\n");
			if(xTaskRemoveFromEventList(&(queue_id->TaskWaitQueue)))
			{
				//PRINTF("resume\n");
				portYIELD_WITHIN_API();
			}
		}
		( void ) xTaskResumeAll();
		return 1;
		//PRINTF("tail %x count %d\n",queue_id->tail,queue_id->count);
	}
	else
	{
		if(timeout==0)
		{
			PRINTF("timeout not defined\n");
			( void ) xTaskResumeAll();
			return 0;
		}
		else
		{
			if(queue_id->time == 0)
			{
				queue_id->time=getTick();
				//PRINTF("Tick: %d\n",queue_id->time);
				//PRINTF("time for queue %d\n",queue_id->time);
			}
		}
		/*
		 *check timeout
		 */
		tick=getTick();
		if((tick-queue_id->time) < timeout)
		{
			uint32_t timewake;
			timeout-=tick-queue_id->time;
			queue_id->time=timeout;

			timewake=tick+timeout;
			//PRINTF("blok thread timeout %d timewake %d\n",timeout,timewake);

			vListInsert(&(queue_id->TaskWaitQueue),getCurrentListTCB());
			delaytask(timewake);
			if( xTaskResumeAll() == pdFALSE )
			{
				//PRINTF("checks resume\n");
				queue_id->time=0;
				portYIELD_WITHIN_API();
			}
		}
	}
	return 0;
}

int queue_get(Queue_Handler* queue_id,void* buffer,uint32_t timeout)
{
	uint32_t check=0;
	uint32_t total=queue_id->length_queue*queue_id->size_queue;
	void* last;
	last=((char*)(queue_id->point+total));
	if(queue_id->point==NULL)
	{
		//PRINTF("check\n");
		return 0;
	}
	if(queue_id->mode>2||queue_id->mode<1)
	{
		queue_id->mode=FIFO;
	}
	if(queue_id->count==0)
	{
		//PRINTF("check1\n");
		check=1;
	}
	if(check==0)
	{
		if(queue_id->mode==1)
		{
			//PRINTF("check2\n");
			( void )memcpy(buffer,queue_id->head,queue_id->size_queue);
			//PRINTF("isi buffer %d\n",(uint32_t)buffer);
			queue_id->head=(Queue_Handler*)((char*)(queue_id->head+queue_id->size_queue));
			if((queue_id->head)>=((Queue_Handler*)(last)))
				queue_id->head=queue_id->point;
		}
		else
		{
			queue_id->tail=(Queue_Handler*)((char*)(queue_id->tail-queue_id->size_queue));
			if((queue_id->tail)<=queue_id->point)
			{
				//PRINTF("check 2");
				queue_id->tail=queue_id->point;
			}
			//PRINTF("queue tail %x\n",queue_id->tail);
			( void )memcpy(buffer,queue_id->tail,queue_id->size_queue);

		}
		queue_id->count--;
		//PRINTF("queue_id  head %x count %d Tick %d\n",queue_id->head,queue_id->count,getTick());
		if(listLIST_IS_EMPTY(&(queue_id->TaskWaitQueue))== 0)
		{
			//PRINTF("list tidak kosong\n");
			if(xTaskRemoveFromEventList(&(queue_id->TaskWaitQueue)))
			{
				//PRINTF("resume\n");
				portYIELD_WITHIN_API();
			}
		}
		return 1;
	}
	else
	{

		vTaskSuspendAll();
		if(timeout==0)
		{
			PRINTF("timeout not defined\n");
			( void ) xTaskResumeAll();
			return 0;
		}
		else
		{
			if(queue_id->time == 0)
			{
				queue_id->time=getTick();
				//PRINTF("Tick: %d\n",queue_id->time);
				//PRINTF("time for queue %d\n",queue_id->time);
			}
		}
		/*
		 *check timeout
		 */
		uint32_t tick=getTick();
		if((tick-queue_id->time) < timeout)//still time
		{
			uint32_t timewake;
			timeout-=tick-queue_id->time;
			queue_id->time=timeout;

			timewake=tick+timeout;
			//PRINTF("blok thread timeout %d timewake %d\n",timeout,timewake);
			vListInsert(&(queue_id->TaskWaitQueue),getCurrentListTCB());
			delaytask(timewake);
			if( xTaskResumeAll() == pdFALSE )
			{
				//PRINTF("checks resume\n");
				queue_id->time=0;
				portYIELD_WITHIN_API();
			}
		}
	}
	return 0;
}

int queue_destroy(Queue_Handler* queue_id)
{
	if(queue_id->point==NULL)
	{
		PRINTF("check\n");
		return 0;
	}
	queue_id->head=NULL;
	queue_id->tail=NULL;
	queue_id->length_queue=0;
	queue_id->size_queue=0;
	queue_id->mode=0;
	queue_id->count=0;
	queue_id->time=0;
	if(Free_TLSF(queue_id->point))
		return 1;
	else
		return 0;
}

int queue_getcount(Queue_Handler* queue_id)
{
	if(queue_id->point == NULL)
	{
		PRINTF("check\n");
		return 0;
	}
	return queue_id->count;
}
