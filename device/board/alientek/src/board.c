#include "board.h"
#include "cmsis_os2.h"
#include "main.h"

const osThreadAttr_t ledSmgTask_attr = {
    .name = "ledSmgTask",
    .stack_size = 128 * 4,
    .priority = (osPriority_t)osPriorityNormal,
};

const osThreadAttr_t uartTask_attr = {
    .name = "uartTask",
    .stack_size = 256 * 4,
    .priority = (osPriority_t)osPriorityNormal,
};

extern void StartLedSmgTask(void *argument);
extern void usart_task(void *argument);
extern void prvUart_Init(void);

/**
 * @brief 扳级初始化
 *        OS 调度之前
 *
 */
void board_init(void)
{
    // board
    prvUart_Init();

    // Thread New
    if (osThreadNew(StartLedSmgTask, NULL, &ledSmgTask_attr) == NULL)
    {
        Error_Handler();
    }
    if (osThreadNew(usart_task, NULL, &uartTask_attr) == NULL)
    {
        Error_Handler();
    }
}

void board_error_handle(void)
{
    __disable_irq();
    while (1)
    {
    }
}