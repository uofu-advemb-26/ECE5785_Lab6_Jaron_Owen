#include <stdio.h>
#include <pico/stdlib.h>
#include <stdint.h>
#include <unity.h>
#include "unity_config.h"
#include "test_thread.h"

#define TASK_SUPERVISOR_TASK_PRIORITY      ( tskIDLE_PRIORITY + 3UL )
#define TASK_SUPERVISOR_TASK_STACK_SIZE configMINIMAL_STACK_SIZE

void supervisor_task(__unused void *params){

    SemaphoreHandle_t semaphore = xSemaphoreCreateBinary();

    TaskHandle_t task_h_1;
    xTaskCreate(task_1, "Task 1", TASK_1_TASK_STACK_SIZE,
                (void*)&semaphore, TASK_1_TASK_PRIORITY, task_h_1);

    TaskHandle_t task_h_2;
    xTaskCreate(task_2, "Task 2", TASK_2_TASK_STACK_SIZE,
                (void*)&semaphore, TASK_2_TASK_PRIORITY, task_h_2);


        // Delay

        // Start high priority task
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
