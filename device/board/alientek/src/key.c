#include "board.h"

#include "stm32f1xx_hal.h"
#include "cmsis_os2.h"

typedef enum
{
    IDLE_STATE = 0,
    PRESS_DETECTED_STATE,
    RELEASE_DETECTED_STATE
} keyStateTypedef;

typedef struct
{
    uint8_t name;
    GPIO_TypeDef *GPIOx;
    uint16_t GPIO_Pin;
    keyStateTypedef state;
    KeyCallback call;
} keyTypedef;

#define KEY0_GPIO_Port GPIOC
#define KEY0_Pin GPIO_PIN_8

#define KEY1_GPIO_Port GPIOC
#define KEY1_Pin GPIO_PIN_9

#define KEY2_GPIO_Port GPIOD
#define KEY2_Pin GPIO_PIN_2

osThreadId_t keyCallHandle;

static osMessageQueueId_t keyQueueHandle;

const osThreadAttr_t KeyCallTask_Attr = {
    .name = "keycall",
    .stack_size = 128 * 4,
    .priority = osPriorityNormal2};

keyTypedef KeyList[3] = {
    {.name = keyName0,
     .GPIOx = KEY0_GPIO_Port,
     .GPIO_Pin = KEY0_Pin,
     .state = IDLE_STATE},
    {.name = keyName1,
     .GPIOx = KEY1_GPIO_Port,
     .GPIO_Pin = KEY1_Pin,
     .state = IDLE_STATE},
    {.name = keyName2,
     .GPIOx = KEY2_GPIO_Port,
     .GPIO_Pin = KEY2_Pin,
     .state = IDLE_STATE}};

static void key_readState(keyTypedef *key);

void StartKeyCallTask(void *argument);

void StartKeyTask(void *argument)
{
    for (;;)
    {
        key_readState(&KeyList[keyName0]);
        key_readState(&KeyList[keyName1]);
        key_readState(&KeyList[keyName2]);
        osDelay(50);
    }
}

void StartKeyCallTask(void *argument)
{
    int key_id;
    for (;;)
    {
        osMessageQueueGet(keyQueueHandle, &key_id, NULL, osWaitForever);
        if (KeyList[key_id].call != NULL && key_id < 3)
        {
            KeyList->call();
        }
    }
}

void key_init(keyNameTypedef key_id, KeyCallback call)
{
    KeyList[key_id].call = call;
}

void prvKey_init(void)
{
    keyQueueHandle = osMessageQueueNew(10, sizeof(uint32_t), NULL);
    if (keyQueueHandle == NULL)
    {
        board_error_handle();
    }

    keyCallHandle = osThreadNew(StartKeyCallTask, NULL, &KeyCallTask_Attr);
    if (keyCallHandle == NULL)
    {
        board_error_handle();
    }
}

static void key_readState(keyTypedef *key)
{
    if (HAL_GPIO_ReadPin(key->GPIOx, key->GPIO_Pin) == 0)
    {
        if (key->state != PRESS_DETECTED_STATE)
        {
            key->state = PRESS_DETECTED_STATE;
            osMessageQueuePut(keyQueueHandle, &key->name, 0, 0);
        }
    }
    else
    {
        key->state = RELEASE_DETECTED_STATE;
    }
}
