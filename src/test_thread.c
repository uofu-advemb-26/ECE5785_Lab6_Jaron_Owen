
#include <stdio.h>

#include "FreeRTOS.h"
#include "task.h"

#include "test_thread.h"
#include "semphr.h"

void high_priority(void *params){

    SemaphoreHandle_t semaphore = (SemaphoreHandle_t)params;

    xSemaphoreTake(semaphore, portMAX_DELAY);

    while(1){
        __nop();
    }


}

void mid_priority(__unused void *params){

    while(1){
        __nop();
    }

}

void low_priority(void *params){

    SemaphoreHandle_t semaphore = (SemaphoreHandle_t)params;

    xSemaphoreGive(semaphore);

}