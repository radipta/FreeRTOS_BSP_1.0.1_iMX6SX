/*
 * Queue_TLSF.h
 *
 *  Created on: Oct 7, 2016
 *      Author: alvin
 */
//#include "heap_tlsf.h"
#ifndef QUEUE_TLSF_H_
#define QUEUE_TLSF_H_
#ifndef INC_FREERTOS_H
	#error "include FreeRTOS.h" must appear in source files before "include queue.h"
#endif
#ifdef __cplusplus
extern "C" {
#endif
#define FIFO 1
#define LIFO 2
typedef struct queuedef
{
	void* head;
	void* tail;
	size_t length_queue;
	size_t size_queue;
	uint32_t time;
	uint32_t count;
	uint32_t mode;
	void* point;
	List_t TaskWaitQueue;
}Queue_Handler;

int queue_create(Queue_Handler* queue_id,size_t length,size_t itemsize,uint32_t mode) PRIVILEGED_FUNCTION;
int queue_send(Queue_Handler* queue_id,void *message,uint32_t timeout) PRIVILEGED_FUNCTION;
int queue_get(Queue_Handler* queue_id,void* buffer,uint32_t timeout) PRIVILEGED_FUNCTION;
int queue_destroy(Queue_Handler* queue_id)PRIVILEGED_FUNCTION;
int queue_getcount(Queue_Handler* queue_id)PRIVILEGED_FUNCTION;
#ifdef __cplusplus
}
#endif

#endif /* QUEUE_TLSF_H_ */
