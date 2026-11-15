#include <stdio.h>
#include "pico/time.h"

uint64_t ulGetRunTimeCounter(void){
    return time_us_64();
}