
#include <stdio.h>

#include "FreeRTOS.h"
#include "task.h"

#include "test_thread.h"


//#define TASK_SUPERVISOR_TASK_PRIORITY      ( tskIDLE_PRIORITY + 1UL )
//#define TASK_SUPERVISOR_TASK_STACK_SIZE configMINIMAL_STACK_SIZE



void high_priority(void *params){

    SemaphoreHandle_t *semaphore = (*SemaphoreHandle_t)params;

    xSemaphoreTake(&semaphore, portMax_Delay);

}

void task_1(void *params){

    SemaphoreHandle_t *semaphore = (*SemaphoreHandle_t)params;

    xSemaphoreTake(&semaphore, portMax_Delay);

}

void task_2(void *params){

    SemaphoreHandle_t *semaphore = (*SemaphoreHandle_t)params;

    xSemaphoreTake(&semaphore, portMax_Delay);

}