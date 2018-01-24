#include <stdio.h>
#include <stdlib.h>
#include "FreeRTOS.h"
#include "task.h"
//#include "stm32f4xx.h"
//#include "stm32f4xx_conf.h"
#include "cmsis_os.h"
#include "Queue_TLSF.h"
#include "Semaphore_TLSF.h"
//#include "diag/Trace.h"

/*
 * Function cycle counter flag
 */
#define flagpilih 0
#define flagqueue 0
#define flagsemph 0

/*
 * allocate and store value inside memory and evaluate each value and address
 */
#define MallocTest1 0

/*
 * allocation and deallocation performance using DWT Cycle counter in Thread 3
 * Debugger
 * please set flagpilih
 */
#define MallocTest2 0

/*Queue test1
 * send and receive data using 1 thread only (Task 2)
 * please set flagqueue and create
 * Debugger
 */
#define QueueTest1 0

/*Queue test 2
 * send and receive data using several thread (task 1,2,3,4)
 * please set flagqueue and create
 * Debugger and Serial
 */
#define QueueTest2 0

/* semaphore test 1
 *  This test using 1 thread to accuire all token (thread 1)
 *  and then check the token that have been obtained after that all token release
 */
#define SemaphoreTest1 1

/*
 *  This test using 3 thread to accuire token (thread 1,2,3) and 1 thread to check token and release all token (thread 4)
 *  Thread 1 Prio realtime
 *  Thread 2 Prio High
 *  Thread 3 Prio Abovenormal
 *  Thread 4 Prio Normal
 *  awal token 3
 *  next 10 token (untuk menambah beban dan melihat blocking)
 */
#define SemaphoreTest2 0
#define SemaphoreTest2mode 0  // Mode Database Test 2 tanpa Thread 4

/*
 * This test using 2 thread or 5 thread to accquire and release token (Thread 1&2)
 * in the second thread after obtaining token, it switch to another thread in order to make a next thread block
 */
#define SemaphoreTest3 0

/* functionality test of task */
#define Thread_1_test 0
#define Thread_2_test 0
#define Thread_3_test 0
#define Thread_4_test 0


/*
 * Priority switching,queue creation, and semaphore creation
 */
#define Difpriority 0
#define CreateQ 0
#define QMode LIFO
#define CreateSem 1

#define CreateQF 0
#define CreateSemF 0

Counting_Semph sem1;
Queue_Handler queue1;
Queue_Handler queue2;

QueueHandle_t queue;
SemaphoreHandle_t sem2;


#if defined(SemaphoreTest1) || defined(SemaphoreTest2)
	#define MaxSem 3
#endif

void* MallocCount(uint32_t size,uint32_t fl,uint32_t sl);
void FreeCount(void* point,uint32_t size);
int Semaphorecount(uint32_t timeout);
int Semaphoregivecount(uint32_t timeout);
//void delays(uint32_t const num);
//void USART_puts(char* ch);
//void USART_putchar(uint8_t ch);
