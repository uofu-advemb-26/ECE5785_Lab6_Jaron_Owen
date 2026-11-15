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




void priority_test_semaphore(void){
    
    printf("supervisor started \r\n");

    xSemaphoreHandle semaphore = xSemaphoreCreateBinary();

    TaskHandle_t task_high;
    xTaskCreate(high_priority, "Task High", TASK_HIGH_TASK_STACK_SIZE,
                semaphore, TASK_HIGH_TASK_PRIORITY, &task_high);

    taskYIELD();
    vTaskDelay(10);

    TaskHandle_t task_mid;
    xTaskCreate(mid_priority, "Task Mid", TASK_MID_TASK_STACK_SIZE,
                NULL, TASK_MID_TASK_PRIORITY, &task_mid);

    taskYIELD();
    vTaskDelay(10);

    TaskHandle_t task_low;
    xTaskCreate(low_priority, "Task Low", TASK_LOW_TASK_STACK_SIZE,
                semaphore, TASK_LOW_TASK_PRIORITY, &task_low);

    taskYIELD();    
    vTaskDelay(100);

    TaskStatus_t xTaskDetailsHigh, xTaskDetailsMid, xTaskDetailsLow;


    vTaskGetInfo(task_high, &xTaskDetailsHigh, pdFALSE, eRunning);    
    unsigned long high_count = xTaskDetailsHigh.ulRunTimeCounter;


    vTaskGetInfo(task_mid, &xTaskDetailsMid, pdFALSE, eRunning);    
    unsigned long mid_count = xTaskDetailsMid.ulRunTimeCounter;

    vTaskGetInfo(task_low, &xTaskDetailsLow, pdTRUE, eInvalid);    
    unsigned long low_count = xTaskDetailsLow.ulRunTimeCounter;

    vSemaphoreDelete(semaphore);
    vTaskDelete(task_high);
    vTaskDelete(task_mid);
    vTaskDelete(task_low);

    printf("High task execution[%d]\n\r Mid task execution[%d]\n\r Low task execution[%d]\n\r", high_count, mid_count, low_count);

    TEST_ASSERT_LESS_OR_EQUAL(100, high_count);
    TEST_ASSERT_GREATER_OR_EQUAL(100000, mid_count);
    TEST_ASSERT_EQUAL(low_count, 0);

}


void priority_test_mutex(void){
    
    printf("supervisor started \r\n");

    xSemaphoreHandle semaphore = xSemaphoreCreateMutex();

    TaskHandle_t task_high;
    xTaskCreate(high_priority, "Task High", TASK_HIGH_TASK_STACK_SIZE,
                semaphore, TASK_HIGH_TASK_PRIORITY, &task_high);

    taskYIELD();
    vTaskDelay(10);

    TaskHandle_t task_mid;
    xTaskCreate(mid_priority, "Task Mid", TASK_MID_TASK_STACK_SIZE,
                NULL, TASK_MID_TASK_PRIORITY, &task_mid);

    taskYIELD();
    vTaskDelay(10);

    TaskHandle_t task_low;
    xTaskCreate(low_priority, "Task Low", TASK_LOW_TASK_STACK_SIZE,
                semaphore, TASK_LOW_TASK_PRIORITY, &task_low);

    taskYIELD();    
    vTaskDelay(100);

    TaskStatus_t xTaskDetailsHigh, xTaskDetailsMid, xTaskDetailsLow;


    vTaskGetInfo(task_high, &xTaskDetailsHigh, pdFALSE, eRunning);    
    unsigned long high_count = xTaskDetailsHigh.ulRunTimeCounter;


    vTaskGetInfo(task_mid, &xTaskDetailsMid, pdFALSE, eRunning);    
    unsigned long mid_count = xTaskDetailsMid.ulRunTimeCounter;

    vTaskGetInfo(task_low, &xTaskDetailsLow, pdTRUE, eInvalid);    
    unsigned long low_count = xTaskDetailsLow.ulRunTimeCounter;

    vSemaphoreDelete(semaphore);
    vTaskDelete(task_high);
    vTaskDelete(task_mid);
    vTaskDelete(task_low);

    printf("High task execution[%d]\n\r Mid task execution[%d]\n\r Low task execution[%d]\n\r", high_count, mid_count, low_count);

    TEST_ASSERT_GREATER_OR_EQUAL(100000, high_count);
    TEST_ASSERT_EQUAL(low_count, 0);
    TEST_ASSERT_EQUAL(mid_count, 0);

}

void test_busy_busy_equal_priority(void){

    TaskHandle_t task_1;
    xTaskCreate(busy_busy, "Task 1", TASK_MID_TASK_STACK_SIZE,
                NULL, TASK_MID_TASK_PRIORITY, &task_1);
    TaskHandle_t task_2;
    xTaskCreate(busy_busy, "Task 2", TASK_MID_TASK_STACK_SIZE,
                NULL, TASK_MID_TASK_PRIORITY, &task_2);
  
    vTaskDelay(10);

    TaskStatus_t xTaskDetails;

    vTaskGetInfo(task_1, &xTaskDetails, pdFALSE, eRunning);    
    unsigned long count_1 = xTaskDetails.ulRunTimeCounter;

    vTaskGetInfo(task_2, &xTaskDetails, pdFALSE, eRunning);    
    unsigned long count_2 = xTaskDetails.ulRunTimeCounter;

    vTaskDelete(task_1);
    vTaskDelete(task_2);

    printf("Busy Busy Equal Priority Task 1 [%d] Task 2 [%d] \n\r", count_1, count_2);

    int difference = abs(count_1 - count_2);
    TEST_ASSERT_LESS_OR_EQUAL(2000, difference);

}

void test_busy_yield_equal_priority(void){

    TaskHandle_t task_1;
    xTaskCreate(busy_yield, "Task 1", TASK_MID_TASK_STACK_SIZE,
                NULL, TASK_MID_TASK_PRIORITY, &task_1);
    TaskHandle_t task_2;
    xTaskCreate(busy_yield, "Task 2", TASK_MID_TASK_STACK_SIZE,
                NULL, TASK_MID_TASK_PRIORITY, &task_2);
  
    vTaskDelay(10);

    TaskStatus_t xTaskDetails;

    vTaskGetInfo(task_1, &xTaskDetails, pdFALSE, eRunning);    
    unsigned long count_1 = xTaskDetails.ulRunTimeCounter;

    vTaskGetInfo(task_2, &xTaskDetails, pdFALSE, eRunning);    
    unsigned long count_2 = xTaskDetails.ulRunTimeCounter;

    vTaskDelete(task_1);
    vTaskDelete(task_2);

    printf("Busy yield Equal Priority Task 1 [%d] Task 2 [%d] \n\r", count_1, count_2);

    int difference = abs(count_1 - count_2);
    TEST_ASSERT_LESS_OR_EQUAL(2000, difference);

}

void test_both_equal_priority(void){

    TaskHandle_t task_1;
    xTaskCreate(busy_yield, "Task 1", TASK_MID_TASK_STACK_SIZE,
                NULL, TASK_MID_TASK_PRIORITY, &task_1);
    TaskHandle_t task_2;
    xTaskCreate(busy_busy, "Task 2", TASK_MID_TASK_STACK_SIZE,
                NULL, TASK_MID_TASK_PRIORITY, &task_2);
  
    vTaskDelay(10);

    TaskStatus_t xTaskDetails;

    vTaskGetInfo(task_1, &xTaskDetails, pdFALSE, eRunning);    
    unsigned long count_1 = xTaskDetails.ulRunTimeCounter;

    vTaskGetInfo(task_2, &xTaskDetails, pdFALSE, eRunning);    
    unsigned long count_2 = xTaskDetails.ulRunTimeCounter;

    printf("Equal Priority: Busy Yield [%d] Busy Busy [%d] \n\r", count_1, count_2);

    vTaskDelete(task_1);
    vTaskDelete(task_2);

    TEST_ASSERT_GREATER_THAN(count_1, count_2);

}


void test_busy_busy_high_first(void){

    TaskHandle_t task_1;
    xTaskCreate(busy_busy, "Task 1", TASK_MID_TASK_STACK_SIZE,
                NULL, TASK_HIGH_TASK_PRIORITY, &task_1);
    TaskHandle_t task_2;
    xTaskCreate(busy_busy, "Task 2", TASK_MID_TASK_STACK_SIZE,
                NULL, TASK_MID_TASK_PRIORITY, &task_2);
  
    vTaskDelay(10);

    TaskStatus_t xTaskDetails;

    vTaskGetInfo(task_1, &xTaskDetails, pdFALSE, eRunning);    
    unsigned long count_1 = xTaskDetails.ulRunTimeCounter;

    vTaskGetInfo(task_2, &xTaskDetails, pdFALSE, eRunning);    
    unsigned long count_2 = xTaskDetails.ulRunTimeCounter;

    vTaskDelete(task_1);
    vTaskDelete(task_2);

    printf("Busy Busy: High Priority [%d] Low Priority [%d] \n\r", count_1, count_2);

    TEST_ASSERT_EQUAL(0, count_2);

}

void test_busy_busy_low_first(void){

    TaskHandle_t task_1;
    xTaskCreate(busy_busy, "Task 1", TASK_MID_TASK_STACK_SIZE,
                NULL, TASK_LOW_TASK_PRIORITY, &task_1);
    TaskHandle_t task_2;
    xTaskCreate(busy_busy, "Task 2", TASK_MID_TASK_STACK_SIZE,
                NULL, TASK_MID_TASK_PRIORITY, &task_2);
  
    vTaskDelay(10);

    TaskStatus_t xTaskDetails;

    vTaskGetInfo(task_1, &xTaskDetails, pdFALSE, eRunning);    
    unsigned long count_1 = xTaskDetails.ulRunTimeCounter;

    vTaskGetInfo(task_2, &xTaskDetails, pdFALSE, eRunning);    
    unsigned long count_2 = xTaskDetails.ulRunTimeCounter;

    vTaskDelete(task_1);
    vTaskDelete(task_2);

    printf("Busy Busy: Low Priority [%d] High Priority [%d] \n\r", count_1, count_2);

    TEST_ASSERT_EQUAL(0, count_1);

}


void test_busy_yield_high_first(void){

    TaskHandle_t task_1;
    xTaskCreate(busy_yield, "Task 1", TASK_MID_TASK_STACK_SIZE,
                NULL, TASK_HIGH_TASK_PRIORITY, &task_1);
    TaskHandle_t task_2;
    xTaskCreate(busy_yield, "Task 2", TASK_MID_TASK_STACK_SIZE,
                NULL, TASK_MID_TASK_PRIORITY, &task_2);
  
    vTaskDelay(10);

    TaskStatus_t xTaskDetails;

    vTaskGetInfo(task_1, &xTaskDetails, pdFALSE, eRunning);    
    unsigned long count_1 = xTaskDetails.ulRunTimeCounter;

    vTaskGetInfo(task_2, &xTaskDetails, pdFALSE, eRunning);    
    unsigned long count_2 = xTaskDetails.ulRunTimeCounter;

    vTaskDelete(task_1);
    vTaskDelete(task_2);

    printf("Busy Yield: High Priority [%d] Low Priority [%d] \n\r", count_1, count_2);

    TEST_ASSERT_EQUAL(0, count_2);

}

void test_busy_yield_low_first(void){

    TaskHandle_t task_1;
    xTaskCreate(busy_yield, "Task 1", TASK_MID_TASK_STACK_SIZE,
                NULL, TASK_LOW_TASK_PRIORITY, &task_1);
    TaskHandle_t task_2;
    xTaskCreate(busy_yield, "Task 2", TASK_MID_TASK_STACK_SIZE,
                NULL, TASK_MID_TASK_PRIORITY, &task_2);
  
    vTaskDelay(10);

    TaskStatus_t xTaskDetails;

    vTaskGetInfo(task_1, &xTaskDetails, pdFALSE, eRunning);    
    unsigned long count_1 = xTaskDetails.ulRunTimeCounter;

    vTaskGetInfo(task_2, &xTaskDetails, pdFALSE, eRunning);    
    unsigned long count_2 = xTaskDetails.ulRunTimeCounter;

    vTaskDelete(task_1);
    vTaskDelete(task_2);

    printf("Busy Yield: Low Priority [%d] High Priority [%d] \n\r", count_1, count_2);

    TEST_ASSERT_EQUAL(0, count_1);

}


void setUp(void) {}

void tearDown(void) {}

int main_task (void)
{
    while (1) {
        vTaskDelay(1000); // Give time for TTY to attach.
        printf("Start tests\n");
        UNITY_BEGIN();
        RUN_TEST(priority_test_semaphore);
        RUN_TEST(priority_test_mutex);
        RUN_TEST(test_busy_busy_equal_priority);
        RUN_TEST(test_busy_yield_equal_priority);
        RUN_TEST(test_both_equal_priority);
        RUN_TEST(test_busy_busy_high_first);
        RUN_TEST(test_busy_busy_low_first);
        RUN_TEST(test_busy_yield_high_first);
        RUN_TEST(test_busy_yield_low_first);
        UNITY_END();
        vTaskDelay(5000);
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