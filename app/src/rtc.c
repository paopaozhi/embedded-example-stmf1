#include "board.h"
#include <stdio.h>

static RtcTimer_TypeDef timer;

void StartRtcTask(void *arg)
{
    // 1. 读

    // rtc_async_read(&timer);
    // printf("hour:%d min:%d\n", timer.hour, timer.min);
    while (1)
    {
        osDelay(10);
    }
}
