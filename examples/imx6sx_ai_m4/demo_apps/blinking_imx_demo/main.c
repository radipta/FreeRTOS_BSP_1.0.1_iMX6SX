
/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
   */

#include "FreeRTOS.h"
#include "task.h"
#include "board.h"
#include "debug_console_imx.h"
#include "gpio_ctrl.h"
#include "hw_timer.h"

#include "kirimtext.h"
#include "Test.h"

#include "Task1.h"
#include "Task2.h"
#include "Task3.h"
#include "Task4.h"

// extern int Init_Task1 (void);
// extern int Init_Task2 (void);

 //extern int Task1 (void);
 //extern int Task2 (void);

uint32_t data;
TickType_t freq=40;

#define BLINKING_INTERVAL_MIN    (100)

static volatile uint32_t blinkingInterval = BLINKING_INTERVAL_MIN;

//*****************************************************************************
//*
//* Function Name: ToggleTask
//* Comments: this task is used to turn toggle on/off LED.
//*
//*****************************************************************************  
void ToggleTask(void *pvParameters)
{
    while (true)
    {
        GPIO_Ctrl_ToggleLed();
        kirim();
        //PRINTF("\n\rjosssssss");
                // Use Hardware timer to get accurate delay   
        Hw_Timer_Delay(blinkingInterval);
    }
}

//*****************************************************************************
//*
//* Function Name: SwitchTask
// Comments: this task is used to change blinking frequency.
//*
//*****************************************************************************  
void SwitchTask(void *pvParameters)
{
    while (true)
    {
        PRINTF("\n\r====== Blinking interval %dms ======\n\r", blinkingInterval);
        GPIO_Ctrl_WaitKeyPressed();
        blinkingInterval += 100;
        if (blinkingInterval > 1000)
            blinkingInterval = BLINKING_INTERVAL_MIN;
        // Delay for 1 second to avoid glitch   
        vTaskDelay(configTICK_RATE_HZ);
    }
}

void delays(uint32_t const num){
    uint32_t i, j;
    for(i=0;i<num;i++)
        for( j=0;j<num;j++)
            asm volatile("nop");
}

// osThreadId Task4Handle;
// osThreadId Task5Handle;

//Semaphore  count function
   
int Semaphorecount(uint32_t timeout)
{
    int   hasil;
        if(flagsemph==0)
        {
            //start_timer();
            hasil=Semaphore_take(&sem1,timeout);
            //tampung=gets();
            //stop_timer();
            PRINTF(" semaphore TLSF : token %d\n",  Semaphore_GetCount(&sem1));
        }
        else
        {
            //start_timer();
            hasil=xQueueGenericSend( ( QueueHandle_t ) ( sem2 ), ((void *)0), ( ( TickType_t ) 0U ), ( ( BaseType_t ) 0 ) );
            //tampung=gets();
            //stop_timer();
            PRINTF(" semaphore FREERTOS : %d\n");
        }
        if(hasil)
            return 1;
    return 0;
}

int Semaphoregivecount(uint32_t timeout)
{
    int   hasil;
        if(flagsemph==0)
        {
            //start_timer();
            hasil=Semaphore_release(&sem1,timeout);
            //tampung=gets();
            //stop_timer();
            PRINTF(" semaphore TLSF : token %d\n",  Semaphore_GetCount(&sem1));
        }
        else
        {
            //start_timer();
            hasil=xQueueGenericReceive( ( QueueHandle_t ) ( sem2 ), ((void *)0), ( timeout ), ( ( BaseType_t ) 0 ) );
            //tampung=gets();
            //stop_timer();
            PRINTF(" semaphore FREERTOS : %d\n");
        }
        if(hasil)
            return 1;
    return 0;
}
//Queue  count function
   
void queuecount(uint32_t message,uint32_t timeout)
{
    int tampung;
    if(flagqueue==0)
    {
        //start_timer();
        queue_send(&queue1,&message,timeout);
        //tampung=gets();
        //stop_timer();
        PRINTF(" TLSF queue: send %d\n",  message);
    }
    else
    {
        //start_timer();
        xQueueSend(queue,&message,timeout);
        //tampung=gets();
        //stop_timer();
        PRINTF(" FREERTOS queue: send %d\n",  message);
    }

}
void queuegetcount(void* message,uint32_t timeout)
{
    int tampung;
    uint32_t test=0;
    if(flagqueue==0)
    {
        //start_timer();
        queue_get(&queue1,&test,timeout);
        //tampung=gets();
        //stop_timer();
        PRINTF("== TLSF get queue: data %d\n",  test);
    }
    else
    {
        //start_timer();
        uint32_t hasil=xQueueGenericReceive(queue,&test,timeout,pdFALSE);
        //tampung=gets();
        //stop_timer();
        if(hasil)
            PRINTF("== FREERTOS get queue: data %d\n",  test);
        else
            PRINTF("gagal\n");
    }

}

//Malloc  count function
   
void* MallocCount(uint32_t size,uint32_t fl,uint32_t sl)
{
    uint32_t* point1;
    int tampung;
    if( flagpilih == 0 )
    {
        //start_timer();
        point1=(uint32_t*)Malloc_TLSF((size_t)size,fl,sl);
        //tampung=gets();
        //stop_timer();
        PRINTF("Malloc = %d = %x\n",size,point1);
    }
    else
    {
        //start_timer();
        point1=(uint32_t*)pvPortMalloc(size);
        //tampung=gets();
        //stop_timer();
        PRINTF("Malloc = %d = %x\n",size,point1);
    }
    return point1;
}

void FreeCount(void* point,uint32_t size)
{
    int tampung;
    if( flagpilih == 0 )
    {
        //start_timer();
        Free_TLSF(point);
        //tampung=gets();
        //stop_timer();
        PRINTF("Free = %d = %x\n",size,point);
    }
    else
    {
        //start_timer();
        vPortFree(point);
        //tampung=gets();
        //stop_timer();
        PRINTF("Free = %d = %x\n",size,point);
    }
}


//*****************************************************************************

// Function Name: main
// Comments: main function, toggle LED and switch the blinking frequency by key.

  
int main(void)
{
    // Initialize board specified hardware.   
    hardware_init();
    Hw_Timer_Init();
    GPIO_Ctrl_Init();
    TLSF_init();// prvHeapInit();
    uint32_t tampung;

    
   
   //initialize task :
    Init_Task1();
    //Init_Task2();
    //Init_Task3();
    //Init_Task4();
    

    // Lists of test macro declaration in file Test.h           
    if(CreateSem)
    {
        int hasil;
        //start_timer();
        hasil=Semaphore_create(&sem1,MaxSem,0,0);
        //=gets();
        //stop_timer();
        if(hasil)
            PRINTF("Create semaphore TLSF success \n");
        else
            PRINTF("Create semaphore TLSF failed\n");
    }
    if(CreateQ)
    {
        int hasil;
        //start_timer();
        hasil=queue_create(&queue1,8,sizeof(uint32_t),QMode);
        //=gets();
        //stop_timer();
        if(hasil)
            PRINTF("Create queue TLSF success  %d\n",hasil);
        else
            PRINTF("Create queue TLSF failed\n");
    }
    if(CreateQF)
    {
        int ;
        //start_timer();
        queue=xQueueCreate(8,sizeof(uint32_t));
        //=gets();
        //stop_timer();
        if(queue)
            PRINTF("Create queue FreeRTOS success \n");
        else
            PRINTF("Create queue FreeRTOS failed\n");
    }
    if(CreateSemF)
    {

        //start_timer();
        sem2=xSemaphoreCreateCounting(MaxSem,0);
        //tampung=gets();
        //stop_timer();
        if(sem2)
            PRINTF("Create semaphore FreeRTOS success : \n");
        else
            PRINTF("Create semaphore FreeRTOS failed\n");
    }           

    vTaskStartScheduler();

    // should never reach this point. 
    while (true);
 
}


