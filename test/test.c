#include <stdio.h>
#include <pico/stdlib.h>
#include <stdint.h>
#include <unity.h>
#include "unity_config.h"
#include "test_thread.h"

#define TASK_SUPERVISOR_TASK_PRIORITY      ( tskIDLE_PRIORITY + 4UL )
#define TASK_SUPERVISOR_TASK_STACK_SIZE configMINIMAL_STACK_SIZE

void supervisor_task(__unused void *params){

    SemaphoreHandle_t semaphore = xSemaphoreCreateBinary();
    xSemaphoreTake(&semaphore, portMax_Delay);

    TaskHandle_t task_high;
    xTaskCreate(high_priority, "Task High", TASK_HIGH_TASK_STACK_SIZE,
                (void*)&semaphore, TASK_HIGH_TASK_PRIORITY, task_high);

    TaskHandle_t task_mid;
    xTaskCreate(mid_priority, "Task Mid", TASK_MID_TASK_STACK_SIZE,
                NULL, TASK_MID_TASK_PRIORITY, task_mid);

    TaskHandle_t task_low;
    xTaskCreate(low_priority, "Task Low", TASK_LOW_TASK_STACK_SIZE,
                (void*)&semaphore, TASK_LOW_TASK_PRIORITY, task_low);

    vTaskDelay(100);


    TaskStatus_t xTaskDetails;
    TaskHandle_t xHandle;

    xHandle = xTaskGetHandle(task_high);
    vTaskGetInfo(xHandle, &xTaskDetails, pdTRUE, eInvalid);    
    unsigned long high_count = xTaskDetails->ulRunTimeCounter;

    xHandle = xTaskGetHandle(task_mid);
    vTaskGetInfo(xHandle, &xTaskDetails, pdTRUE, eInvalid);    
    unsigned long mid_count = xTaskDetails->ulRunTimeCounter;

    xHandle = xTaskGetHandle(task_low);
    vTaskGetInfo(xHandle, &xTaskDetails, pdTRUE, eInvalid);    
    unsigned long low_count = xTaskDetails->ulRunTimeCounter;

    vSemaphoreDelete(semaphore);
    xTaskDelete(task_high);
    xTaskDelete(task_mid);
    xTaskDelete(task_low);

    printf("High task execution[%d]\n\r Mid task execution[%d]\n\r Low task execution[%d]\n\r", high_count, mid_count, low_count);

    xTaskDelete(NULL);
}

void priority_test(void){
    
    TaskHandle_t supervisor_handle;
    xTaskCreate(supervisor_task, "Supervisor Task", TASK_SUPERVISOR_TASK_STACK_SIZE,
                NULL, TASK_SUPERVISOR_TASK_PRIORITY, supervisor_handle);

    printf("Task deleted :)");

}

void setUp(void) {



}

void tearDown(void) {}

//void test_variable_assignment()
//{
//    int x = 1;
//    TEST_ASSERT_TRUE_MESSAGE(x == 1,"Variable assignment failed.");
//}


int main (void)
{
    stdio_init_all();
    while (1) {
        sleep_ms(5000); // Give time for TTY to attach.
        printf("Start tests\n");
        UNITY_BEGIN();
        RUN_TEST(test_variable_assignment);
        RUN_TEST(test_multiplication);
        sleep_ms(5000);
        UNITY_END();
    }
}
