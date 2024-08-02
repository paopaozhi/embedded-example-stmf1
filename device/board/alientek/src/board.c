#include "board.h"
#include "cmsis_os2.h"
#include "main.h"

const osThreadAttr_t keyTask_attr = {
    .name = "keyTask",
    .stack_size = 128 * 4,
    .priority = (osPriority_t)osPriorityNormal2,
};

const osThreadAttr_t ledSmgTask_attr = {
    .name = "ledSmgTask",
    .stack_size = 128 * 4,
    .priority = (osPriority_t)osPriorityNormal,
};

const osThreadAttr_t uartTask_attr = {
    .name = "uartTask",
    .stack_size = 150 * 4,
    .priority = (osPriority_t)osPriorityNormal7,
};

extern void StartKeyTask(void *argument);
extern void StartLedSmgTask(void *argument);
extern void usart_task(void *argument);
extern void prvUart_Init(void);
extern void prvKey_init(void);

/**
 * @brief 扳级初始化
 *        OS 调度之前
 *
 */
void board_init(void)
{
    // board
    prvUart_Init();

    prvKey_init();

    // Thread New
    if (osThreadNew(usart_task, NULL, &uartTask_attr) == NULL)
    {
        Error_Handler();
    }

    if (osThreadNew(StartKeyTask, NULL, &keyTask_attr) == NULL)
    {
        Error_Handler();
    }
    if (osThreadNew(StartLedSmgTask, NULL, &ledSmgTask_attr) == NULL)
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