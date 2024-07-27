#include <stdio.h>
#include <string.h>

#include "board.h"
#include "main.h"
#include "FreeRTOS.h"

static uint8_t buf[128];
static uint32_t len = 0;

const osThreadAttr_t rtc_arrtTask = {
    .stack_size = 512 * 4,
    .priority = osPriorityBelowNormal,
};

extern void StartRtcTask(void *arg);

void app(void)
{
    // printf("sum free:%d\r\n",xPortGetFreeHeapSize());

    memset(buf, 0, 128);
    // uart_async_read(uart1, buf, len);
    // printf("buf:%s\n", buf);

    osThreadNew(StartRtcTask, NULL, &rtc_arrtTask);

    while (1)
    {
        HAL_GPIO_TogglePin(LED0_GPIO_Port, LED0_Pin);
        osDelay(500);
    }
}
