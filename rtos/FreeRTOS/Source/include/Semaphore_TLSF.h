#ifndef SEMAPHORE_TLSF_H_
#define SEMAPHORE_TLSF_H_

#ifndef INC_FREERTOS_H
	#error "include FreeRTOS.h" must appear in source files before "include queue.h"
#endif
#ifdef __cplusplus
extern "C" {
#endif

#define SemphObj_id 2;
typedef struct Linked{
	void* prev;
	uint32_t tasktcb;
	void* next;
}LinkedTask;

typedef struct Semaphore{
	void* address;
	int Obj_id;
	uint32_t token;
	uint32_t maxtoken;
	uint32_t time;
	uint32_t strmode;
	List_t TaskWaitSemaphore;
	LinkedTask *linktask;
	LinkedTask *head;
	LinkedTask *tail;
}Counting_Semph;

int Semaphore_create(Counting_Semph* id,uint32_t maxtokens,uint32_t initial_count,uint32_t StoreMode)PRIVILEGED_FUNCTION;
int Semaphore_take(Counting_Semph* id,uint32_t timeout)PRIVILEGED_FUNCTION;
int Semaphore_release(Counting_Semph* id,uint32_t timeout)PRIVILEGED_FUNCTION;
uint32_t Semaphore_GetCount(Counting_Semph* id)PRIVILEGED_FUNCTION;
uint32_t Semaphore_destroy(Counting_Semph* id)PRIVILEGED_FUNCTION;
void getTaskAccquire(Counting_Semph* id)PRIVILEGED_FUNCTION;

#ifdef __cplusplus
}
#endif

#endif /* SEMAPHORE_TLSF_H_ */
