#include <stdio.h>
#include <string.h>

#include "FreeRTOS.h"
#include "board.h"
#include "cmsis_os2.h"
#include "main.h"
#include "task.h"
#include "timers.h"

typedef struct
{
    uint8_t buf[128];
    uint8_t buf_len;
    uint8_t state;

    xTimerHandle receive_timerHandle;

    osEventFlagsId_t readOk_Event;
} Uart_TypeDef;

#define UART_RECEIVE_OK 0x00000001U
#define UART_COPY_OK 0x00000002U

extern UART_HandleTypeDef huart1;

static uint8_t UartId = 0xff;

static osSemaphoreId_t read_Sema;

static Uart_TypeDef uartList[3];

static void uartReceiveHandle(TimerHandle_t xTimer)
{
    // send
    osEventFlagsSet(uartList[UartId].readOk_Event, UART_RECEIVE_OK);
}

static void uart_new_timer(Uart_TypeDef *uart)
{
    uart->receive_timerHandle = xTimerCreate(NULL, 5, pdFALSE, (void *)0, uartReceiveHandle);
    if (uart->receive_timerHandle == NULL)
    {
        Error_Handler();
    }
    xTimerStop(uart->receive_timerHandle, portMAX_DELAY);
}

void prvUart_Init(void)
{
    uint8_t uart_id_len = sizeof(uartList) / sizeof(Uart_TypeDef) - 1;
    Uart_TypeDef *pUart;
    read_Sema = osSemaphoreNew(1, 0, NULL);

    for (int i = 0; i < uart_id_len; i++)
    {
        pUart = &uartList[i];
        uart_new_timer(pUart);

        pUart->readOk_Event = osEventFlagsNew(NULL);

        if (pUart->readOk_Event == NULL)
        {
            board_error_handle();
        }

        pUart->state = 0;
        pUart->buf_len = 0;
        memset(pUart->buf, 0, 128);

        pUart = NULL;
    }
}

void usart_task(void *argument)
{
    Uart_TypeDef *pUart;

    for (;;)
    {
        osSemaphoreAcquire(read_Sema, osWaitForever);

        pUart = &uartList[UartId];

        switch (UartId)
        {
        case uart1:
            HAL_UART_Receive_IT(&huart1, pUart->buf, 1);
            osEventFlagsWait(pUart->readOk_Event, UART_COPY_OK, osFlagsWaitAny, osWaitForever);

            HAL_UART_Abort_IT(&huart1);
            break;

        default:
            continue;
            break;
        }

        pUart->state = 0;
        pUart->buf_len = 0;
        memset(pUart->buf, 0, 128);
        UartId = 0xff;
        pUart = NULL;
    }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART1)
    {
        Uart_TypeDef *pUart = &uartList[0];
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        xTimerStartFromISR(pUart->receive_timerHandle, &xHigherPriorityTaskWoken);
        pUart->state = 1;
        pUart->buf_len++;
        HAL_UART_Receive_IT(huart, (pUart->buf + pUart->buf_len), 1);
    }
    else if (huart->Instance == USART2)
    {
        // BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        // xTimerStartFromISR(usartTimerHandle, &xHigherPriorityTaskWoken);
        // USART1_State = 1;
        // USART1_BufLen++;
        // HAL_UART_Receive_IT(huart, (USART1_Buf + USART1_BufLen), 1);
    }
    else if (huart->Instance == USART3)
    {
        // BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        // xTimerStartFromISR(usartTimerHandle, &xHigherPriorityTaskWoken);
        // USART1_State = 1;
        // USART1_BufLen++;
        // HAL_UART_Receive_IT(huart, (USART1_Buf + USART1_BufLen), 1);
    }
    else
    {
        Error_Handler();
    }
}

osStatus_t uart_async_read(UartId_TypeDef uart, uint8_t *buf, uint32_t len)
{
    Uart_TypeDef *pUart = &uartList[uart];
    UartId = uart;
    osEventFlagsId_t pEvent = pUart->readOk_Event;
    uint32_t result;

    osSemaphoreRelease(read_Sema);

    result = osEventFlagsWait(pEvent, UART_RECEIVE_OK, osFlagsWaitAny, osWaitForever);
    if (pUart->buf_len >= 128)
    {
        return osError;
    }
    strncpy((char *)buf, (char *)pUart->buf, pUart->buf_len);
    len = pUart->buf_len;
    osEventFlagsSet(pUart->readOk_Event, UART_COPY_OK);
    return osOK;
}

int __io_putchar(int ch)
{
    HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xff);

    return ch;
}
