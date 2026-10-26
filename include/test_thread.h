#pragma once

#define TASK_HIGH_PRIORITY_TASK_PRIORITY      ( tskIDLE_PRIORITY + 2UL )
#define TASK_HIGH_PRIORITY_TASK_STACK_SIZE configMINIMAL_STACK_SIZE

#define TASK_1_TASK_PRIORITY      ( tskIDLE_PRIORITY + 1UL )
#define TASK_1_TASK_STACK_SIZE configMINIMAL_STACK_SIZE

#define TASK_2_TASK_PRIORITY      ( tskIDLE_PRIORITY + 1UL )
#define TASK_2_TASK_STACK_SIZE configMINIMAL_STACK_SIZE


void high_priority(void *params);
void task_1(void *params);
void task_2(void *params);