/*
 * Semaphore_TLSF.c
 *
 *  Created on: Mar 26, 2017
 *      Author: alvin
 */
#include "FreeRTOS.h"
#include "task.h"
#include "Semaphore_TLSF.h"
#include <string.h>

void Createlist(Counting_Semph* id);
void insertTask(uint32_t addr, Counting_Semph* id);
void deleteTask(Counting_Semph* id);

int Semaphore_create(Counting_Semph *id,uint32_t maxtokens,uint32_t initial_count,uint32_t storemode)
{
	void* addr;

	if(maxtokens == NULL||maxtokens == 0)
	{
		PRINTF("tes1");
		return 0;
	}
	if(initial_count > maxtokens)
	{
		PRINTF("tes2");
		return 0;
	}
	if(storemode >= 1)
		storemode=1;
	else
		storemode=0;
	addr=(void*)Malloc_TLSF(sizeof(Counting_Semph),NONE,NONE);
	//PRINTF("Alamat TAsk %x %x\n",&id->linktask,id->linktask);
	id->address=addr;
	//PRINTF("nilai addr %x %x\n",id->address,&id->address);
	id=(Counting_Semph*)addr;
	//PRINTF("posisi Semp %x %x\n",id,&id);
	id->time=0;
	id->maxtoken=maxtokens;
	id->token=initial_count;

	id->linktask=NULL;
	id->head=NULL;
	id->strmode=storemode;
	vListInitialise( &(id->TaskWaitSemaphore) );
	//PRINTF("posisi mtoken %x tok %x\n",&id->maxtoken,&id->token);
	//PRINTF("nilai maxtoken %d nilai token %d\n\n",id->maxtoken,id->token);
	return 1;
}

int Semaphore_take(Counting_Semph *id,uint32_t timeout)
{
	uint32_t pilih=0;
	id=(Counting_Semph*)id->address;
	//PRINTF("posisi Semp %x\n",id);
	taskENTER_CRITICAL();
	if(id==NULL)
	{
		taskEXIT_CRITICAL();
		return 0;
	}
	uint32_t tokentemp=id->token;
	tokentemp++;
	if(tokentemp > id->maxtoken)
	{
		if(id->strmode==0)
		{
			pilih=1;
			PRINTF("pilih 1\n");
		}

		else if(id->strmode==1&&checkSemTCB()==0)
			pilih=1;
	}
	if(pilih==1)
	{
		PRINTF("masuk blok\n");
		id->token=tokentemp;
		if(timeout==0)
		{
			PRINTF("Timeout not defined\n");
			taskEXIT_CRITICAL();
			return 0;
		}
		if(id->time==0)
		{
			id->time=getTick();
		}
		PRINTF("time %d\n",id->time);
		PRINTF("timeout %d\n",timeout);
		uint32_t tick=getTick();
		if(id->time > timeout)
		{
			id->time=tick+timeout;
		}
		taskEXIT_CRITICAL();
		vTaskSuspendAll();
		uint32_t timewake;
		timeout-=tick-id->time;
		id->time=timeout;
		timewake=tick+timeout;
		PRINTF("blok thread timeout %d timewake %d\n",timeout,timewake);
		vListInsert(&(id->TaskWaitSemaphore),getCurrentListTCB());
		delaytask(timewake);
		if( xTaskResumeAll() == pdFALSE )
		{
			id->time=0;
			portYIELD_WITHIN_API();
		}
		return 0;
	}
	else
	{
		taskEXIT_CRITICAL();
		LinkedTask *Task;
		//PRINTF("token %d\n",id->token);
		//PRINTF("TASK ADDR %x %x\n",&Task,Task);
		//PRINTF("TAsk %x\n",Task->next);
		if(id->strmode==1)
		{
			if(checkSemTCB()==0)
			{
				uint32_t temp=(uint32_t)getCurrentTCB();
				Createlist(&id);
				insertTask(temp,&id);
				id->token=tokentemp;
				Task = id->linktask;
			}
			else
			{
				//PRINTF("TCB %x\n",getCurrentTCB());
				return 0;
			}
		}
		else
		{
			id->token=tokentemp;
			Task = id->linktask;
		}
	}
	//taskEXIT_CRITICAL();
	return 1;
}

int Semaphore_release(Counting_Semph* id,uint32_t timeout)
{
	//PRINTF("release\n");
	id=(Counting_Semph*)id->address;
	taskENTER_CRITICAL();
	if(id==NULL)
	{
		taskEXIT_CRITICAL();
		return 0;
	}
	if(id->token > id->maxtoken)
	{
		id->token=id->maxtoken;
		//PRINTF("token jadi max %d\n",id->token);
	}
	int tokentemp=id->token;
	tokentemp-=1;
	if(tokentemp < 0)
	{
		id->token=0;
		//PRINTF("masuk token %d\n",id->token);
		if(timeout==0)
		{
			PRINTF("Timeout not defined");
			taskEXIT_CRITICAL();
			return 0;
		}
		if(id->time==0)
		{
			id->time=getTick();
		}
		uint32_t tick=getTick();
		if(id->time > timeout)
		{
			id->time=tick+timeout;
			PRINTF("time %d\n",id->time);
			PRINTF("tick %d\n",tick);
		}
		else
		{
			PRINTF("time %d\n",id->time);
						PRINTF("tick %d\n",tick);
		}
		taskEXIT_CRITICAL();
		vTaskSuspendAll();
		uint32_t timewake;
		timeout-=tick-id->time;
		id->time=timeout;
		timewake=tick+timeout;
		//PRINTF("blok thread timeout %d timewake %d\n",timeout,timewake);
		vListInsert(&(id->TaskWaitSemaphore),getCurrentListTCB());
		delaytask(timewake);
		if( xTaskResumeAll() == pdFALSE )
		{
			id->time=0;
			//PRINTF("yield\n");
			portYIELD_WITHIN_API();
		}
		return 0;
	}
	else
	{
		taskEXIT_CRITICAL();
		id->token=tokentemp;
		//PRINTF("TOKEN %d\n",id->token);
		if(id->strmode==1)
			deleteTask(&id);
		if(listLIST_IS_EMPTY(&(id->TaskWaitSemaphore))== 0)
		{
			//PRINTF("list tidak kosong\n");
			if(xTaskRemoveFromEventList(&(id->TaskWaitSemaphore)))
			{
				//PRINTF("resume\n");
				portYIELD_WITHIN_API();
			}
		}
	}
	return 1;
}


/*

int Semaphore_take(Counting_Semph *id,uint32_t timeout)
{
	uint32_t pilih=0;
	id=(Counting_Semph*)id->address;
	//PRINTF("posisi Semp %x\n",id);
	taskENTER_CRITICAL();
	if(id==NULL)
	{
		taskEXIT_CRITICAL();
		return 0;
	}
	uint32_t tokentemp=id->token;
	tokentemp++;
	if(tokentemp > id->maxtoken)
	{
		if(id->strmode==0)
		{
			pilih=1;
			PRINTF("pilih 1\n");
		}

		else if(id->strmode==1&&checkSemTCB()==0)
			pilih=1;
	}
	if(pilih==1)
	{
		PRINTF("masuk blok\n");
		id->token=tokentemp;
		if(timeout==0)
		{
			PRINTF("Timeout not defined\n");
			taskEXIT_CRITICAL();
			return 0;
		}
		if(id->time==0)
		{
			id->time=getTick();
		}
		PRINTF("time %d\n",id->time);
		PRINTF("timeout %d\n",timeout);
		uint32_t tick=getTick();
		if(id->time > timeout)
		{
			id->time=tick+timeout;
		}
		taskEXIT_CRITICAL();
		vTaskSuspendAll();
		uint32_t timewake;
		timeout-=tick-id->time;
		id->time=timeout;
		timewake=tick+timeout;
		PRINTF("blok thread timeout %d timewake %d\n",timeout,timewake);
		vListInsert(&(id->TaskWaitSemaphore),getCurrentListTCB());
		delaytask(timewake);
		if( xTaskResumeAll() == pdFALSE )
		{
			id->time=0;
			portYIELD_WITHIN_API();
		}
		return 0;
	}
	else
	{
		taskEXIT_CRITICAL();
		LinkedTask *Task;
		//PRINTF("token %d\n",id->token);
		//PRINTF("TASK ADDR %x %x\n",&Task,Task);
		//PRINTF("TAsk %x\n",Task->next);
		if(id->strmode==1)
		{
			if(checkSemTCB()==0)
			{
				uint32_t temp=(uint32_t)getCurrentTCB();
				Createlist(&id);
				insertTask(temp,&id);
				id->token=tokentemp;
				Task = id->linktask;
			}
			else
			{
				//PRINTF("TCB %x\n",getCurrentTCB());
				return 0;
			}
		}
		else
		{
			id->token=tokentemp;
			Task = id->linktask;
		}
	}
	//taskEXIT_CRITICAL();
	return 1;
}

int Semaphore_release(Counting_Semph* id,uint32_t timeout)
{
	//PRINTF("release\n");
	id=(Counting_Semph*)id->address;
	taskENTER_CRITICAL();
	if(id==NULL)
	{
		taskEXIT_CRITICAL();
		return 0;
	}
	if(id->token > id->maxtoken)
	{
		id->token=id->maxtoken;
		//PRINTF("token jadi max %d\n",id->token);
	}
	int tokentemp=id->token;
	tokentemp-=1;
	if(tokentemp < 0)
	{
		id->token=0;
		//PRINTF("masuk token %d\n",id->token);
		if(timeout==0)
		{
			PRINTF("Timeout not defined");
			taskEXIT_CRITICAL();
			return 0;
		}
		if(id->time==0)
		{
			id->time=getTick();
		}
		uint32_t tick=getTick();
		if(id->time > timeout)
		{
			id->time=tick+timeout;
			PRINTF("time %d\n",id->time);
			PRINTF("tick %d\n",tick);
		}
		else
		{
			PRINTF("time %d\n",id->time);
						PRINTF("tick %d\n",tick);
		}
		taskEXIT_CRITICAL();
		vTaskSuspendAll();
		uint32_t timewake;
		timeout-=tick-id->time;
		id->time=timeout;
		timewake=tick+timeout;
		//PRINTF("blok thread timeout %d timewake %d\n",timeout,timewake);
		vListInsert(&(id->TaskWaitSemaphore),getCurrentListTCB());
		delaytask(timewake);
		if( xTaskResumeAll() == pdFALSE )
		{
			id->time=0;
			//PRINTF("yield\n");
			portYIELD_WITHIN_API();
		}
		return 0;
	}
	else
	{
		taskEXIT_CRITICAL();
		id->token=tokentemp;
		//PRINTF("TOKEN %d\n",id->token);
		if(id->strmode==1)
			deleteTask(&id);
		if(listLIST_IS_EMPTY(&(id->TaskWaitSemaphore))== 0)
		{
			//PRINTF("list tidak kosong\n");
			if(xTaskRemoveFromEventList(&(id->TaskWaitSemaphore)))
			{
				//PRINTF("resume\n");
				portYIELD_WITHIN_API();
			}
		}
	}
	return 1;
}

*/

uint32_t Semaphore_GetCount(Counting_Semph* id)
{
	id=(Counting_Semph*)id->address;
	if(id->token > id->maxtoken)
	{
		id->token = id->maxtoken;
	}
	return id->token;
}
uint32_t Semaphore_destroy(Counting_Semph* id)
{
	void* addr= id->address;
	id->maxtoken=0;
	id->time=0;
	id->token=0;
	Free_TLSF(addr);
	return 1;
}
void Createlist(Counting_Semph* id)
{
	id=(Counting_Semph*)id->address;
	if(id->linktask==NULL)
	{
		id->linktask=(LinkedTask *)Malloc_TLSF(sizeof(LinkedTask),NONE,NONE);
		//PRINTF("Alamat TAsk %x\n",id->linktask);
		id->head=id->linktask;
		id->tail=id->linktask;
		//PRINTF("Alamat head %x\n",id->head);
		id->linktask->next=NULL;
		id->linktask->prev=NULL;
	}
	else
	{
		id->linktask=(LinkedTask *)Malloc_TLSF(sizeof(LinkedTask),NONE,NONE);
		//PRINTF("Alamat2 TAsk %x\n",id->linktask);
		id->tail->next=id->linktask;
		//PRINTF("Alamat next %x | %x\n",id->head->next,id->tail->next);
		id->linktask->prev=id->tail;
		//PRINTF("Alamat prev %x\n",id->linktask->prev);
		id->linktask->next=NULL;
		id->tail=id->linktask;
	}
}
void insertTask(uint32_t addr, Counting_Semph* id)
{
	id=(Counting_Semph*)id->address;
	//PRINTF("insert %x %x addr %x\n",id->linktask,id->linktask,addr);
	id->linktask->tasktcb=addr;
	insertSemTCB(id->linktask);
}
void deleteTask(Counting_Semph* id)
{
	void* addr;
	LinkedTask *ptr,*ptrprev,*ptrnext;
	id=(Counting_Semph*)id->address;
	addr=getSemTCB();
	insertSemTCB(NULL);
	if(addr!=0)
	{
		ptr=addr;
		//PRINTF("pointer %x\n",ptr);
		if(ptr->prev==NULL)
		{
			addr=id->head;
			id->head=id->head->next;
			//PRINTF("addr before %x\n",addr);
			//PRINTF("head %x\n",id->head);
		}
		else
		{
			ptrnext=ptr->next;
			//PRINTF("ptrnext %x\n",ptrnext);
			ptrprev=ptr->prev;
			//PRINTF("ptrprev %x\n",ptrprev);
			ptrprev->next=ptr->next;
			//PRINTF("ptrprev -> next %x\n",ptrprev->next);
			ptrnext->prev=ptrprev;
			//PRINTF("ptrprev ->prev %x\n",ptrprev->prev);
		}
	//void* addr=id->head;
	//PRINTF("addr before %x\n",id->head);
	//id->head=id->head->next;
	Free_TLSF(addr);
	}
	//PRINTF("Next addr %x\n",id->head);
}
void getTaskAccquire(Counting_Semph* id)
{
	PRINTF("======get task accquire=======\n");
	id=(Counting_Semph*)id->address;
	uint32_t temp=0;
	LinkedTask *Tasks;
	Tasks=id->head;
	PRINTF("Alamat %x %x\n",Tasks,Tasks->next);

	while(Tasks != NULL)
	{
		//(void)memcpy(&temp,Tasks->tasktcb,sizeof(uint32_t));
		temp=Tasks->tasktcb;
		PRINTF("TCB %x\n",temp);
		Tasks=Tasks->next;
		PRINTF("alamat next %x\n",Tasks);
	}

}
