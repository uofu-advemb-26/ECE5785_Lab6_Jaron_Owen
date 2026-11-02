#include <stdio.h>
#include <pico/stdlib.h>
#include <stdint.h>
#include <unity.h>
#include "unity_config.h"
#include "test_thread.h"


#include "FreeRTOS.h"
#include "task.h"

#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "pico/cyw43_arch.h"
#include "semphr.h"

#define TASK_SUPERVISOR_TASK_PRIORITY      ( tskIDLE_PRIORITY + 4UL )
#define TASK_SUPERVISOR_TASK_STACK_SIZE configMINIMAL_STACK_SIZE
#define MAIN_TASK_PRIORITY      ( tskIDLE_PRIORITY + 5UL )
#define MAIN_TASK_STACK_SIZE configMINIMAL_STACK_SIZE



void supervisor_task(__unused void *params){

    printf("supervisor started \r\n");

    xSemaphoreHandle semaphore = xSemaphoreCreateBinary();
    //xSemaphoreGive(semaphore);
    //xSemaphoreTake(semaphore, portMAX_DELAY);

    TaskHandle_t task_high;
    xTaskCreate(high_priority, "Task High", TASK_HIGH_TASK_STACK_SIZE,
                semaphore, TASK_HIGH_TASK_PRIORITY, task_high);

    TaskHandle_t task_mid;
    xTaskCreate(mid_priority, "Task Mid", TASK_MID_TASK_STACK_SIZE,
                NULL, TASK_MID_TASK_PRIORITY, task_mid);

    TaskHandle_t task_low;
    xTaskCreate(low_priority, "Task Low", TASK_LOW_TASK_STACK_SIZE,
                semaphore, TASK_LOW_TASK_PRIORITY, task_low);

    vTaskDelay(100);


    TaskStatus_t xTaskDetails;
    TaskHandle_t xHandle;

    xHandle = xTaskGetHandle(task_high);
    vTaskGetInfo(xHandle, &xTaskDetails, pdTRUE, eInvalid);    
    unsigned long high_count = xTaskDetails.ulRunTimeCounter;

    xHandle = xTaskGetHandle(task_mid);
    vTaskGetInfo(xHandle, &xTaskDetails, pdTRUE, eInvalid);    
    unsigned long mid_count = xTaskDetails.ulRunTimeCounter;

    xHandle = xTaskGetHandle(task_low);
    vTaskGetInfo(xHandle, &xTaskDetails, pdTRUE, eInvalid);    
    unsigned long low_count = xTaskDetails.ulRunTimeCounter;

    vSemaphoreDelete(semaphore);
    vTaskDelete(task_high);
    vTaskDelete(task_mid);
    vTaskDelete(task_low);

    printf("High task execution[%d]\n\r Mid task execution[%d]\n\r Low task execution[%d]\n\r", high_count, mid_count, low_count);

    vTaskDelay(100);
    printf("Task deleted :)\r\n");
    vTaskDelete(NULL);
}

void priority_test(void){
    
    TaskHandle_t supervisor_handle;
    xTaskCreate(supervisor_task, "Supervisor Task", TASK_SUPERVISOR_TASK_STACK_SIZE,
                NULL, TASK_SUPERVISOR_TASK_PRIORITY, supervisor_handle);

    vTaskDelay(400);

    

}

void setUp(void) {}

void tearDown(void) {}

int main_task (void)
{
    while (1) {
        vTaskDelay(1000); // Give time for TTY to attach.
        printf("Start tests\n");
        UNITY_BEGIN();
        //RUN_TEST(test_variable_assignment);
        //RUN_TEST(test_multiplication);
        RUN_TEST(priority_test);
        vTaskDelay(1000);
        UNITY_END();
    }
}

int main (void)
{
    stdio_init_all();
    TaskHandle_t main_task_h;
    xTaskCreate(main_task, "Main Task", MAIN_TASK_STACK_SIZE,
        NULL, MAIN_TASK_PRIORITY, main_task_h);

    vTaskStartScheduler();

}