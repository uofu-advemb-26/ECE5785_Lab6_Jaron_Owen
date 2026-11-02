

#define TASK_HIGH_TASK_PRIORITY      ( tskIDLE_PRIORITY + 3UL )
#define TASK_HIGH_TASK_STACK_SIZE configMINIMAL_STACK_SIZE

#define TASK_MID_TASK_PRIORITY      ( tskIDLE_PRIORITY + 2UL )
#define TASK_MID_TASK_STACK_SIZE configMINIMAL_STACK_SIZE

#define TASK_LOW_TASK_PRIORITY      ( tskIDLE_PRIORITY + 1UL )
#define TASK_LOW_TASK_STACK_SIZE configMINIMAL_STACK_SIZE


void high_priority(void *params);
void mid_priority(__unused void *params);
void low_priority(void *params);