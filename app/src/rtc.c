#include "board.h"
#include <stdio.h>
#include "led_smg.h"
#include "cJSON.h"
#include "cmsis_os2.h"
#include <string.h>

typedef enum
{
    unpack_ok = 0,
    unpack_error,
} PackStatus;

const osThreadAttr_t RtcReceive_Attr = {
    .stack_size = 150 * 4,
    .priority = osPriorityNormal1,
};

// 0: show timer 1: show date
static uint8_t isShowMode = 0;

static uint8_t Rtc_Buf[10];
static uint8_t RtcReceiveBuf[128];

static RtcTimer_TypeDef Timer;

static void unpack(char *str, RtcTimer_TypeDef *timer);

static void StartRtcReceiveTask(void *arg);
static void Key1_CallBack(void);

void StartRtcTask(void *arg)
{
    if (osThreadNew(StartRtcReceiveTask, NULL, &RtcReceive_Attr) == NULL)
    {
        printf("init rtc error!\n");
        osThreadExit();
    }
    key_init(keyName0, Key1_CallBack);
    while (1)
    {
        rtc_async_read(&Timer);
        memset(Rtc_Buf, 0, sizeof(Rtc_Buf));

        if (isShowMode == 0)
        {
            sprintf((char *)Rtc_Buf, "%02d-%02d-%02d", Timer.hour, Timer.min, Timer.second);
        }
        else
        {
            sprintf((char *)Rtc_Buf, "20%02d%02d%02d", Timer.year, Timer.month, Timer.day);
        }
        LED_Str(Rtc_Buf);
        osDelay(1000);
    }
}

static void StartRtcReceiveTask(void *arg)
{
    uint8_t len;
    RtcTimer_TypeDef timer;
    while (1)
    {
        memset(RtcReceiveBuf, 0, sizeof(RtcReceiveBuf));
        uart_async_read(uart1, RtcReceiveBuf, len);
        printf("rtc_json: %s\n", RtcReceiveBuf);
        unpack(RtcReceiveBuf, &timer);
        rtc_async_write(&timer);
    }
}

static void Key1_CallBack(void)
{
    isShowMode = !isShowMode;
}

static void unpack(char *str, RtcTimer_TypeDef *timer)
{
    cJSON *json_Handle;

    cJSON *cjson_date = NULL;

    cJSON *cjson_Year = NULL;
    cJSON *cjson_Month = NULL;
    cJSON *cjson_Date = NULL;
    cJSON *cjson_WeekDay = NULL;

    cJSON *cjson_time = NULL;

    cJSON *cjson_Hours = NULL;
    cJSON *cjson_Minutes = NULL;
    cJSON *cjson_Seconds = NULL;

    json_Handle = cJSON_Parse(str);

    cjson_date = cJSON_GetObjectItem(json_Handle, "date");

    cjson_Year = cJSON_GetObjectItem(cjson_date, "year");
    cjson_Month = cJSON_GetObjectItem(cjson_date, "month");
    cjson_Date = cJSON_GetObjectItem(cjson_date, "date");
    cjson_WeekDay = cJSON_GetObjectItem(cjson_date, "weekDay");

    timer->year = cjson_Year->valueint;
    timer->month = cjson_Month->valueint;
    timer->day = cjson_Date->valueint;
    // timer.WeekDay = cjson_WeekDay->valueint;

    cjson_time = cJSON_GetObjectItem(json_Handle, "time");

    cjson_Hours = cJSON_GetObjectItem(cjson_time, "hours");
    cjson_Minutes = cJSON_GetObjectItem(cjson_time, "minutes");
    cjson_Seconds = cJSON_GetObjectItem(cjson_time, "seconds");

    timer->hour = cjson_Hours->valueint;
    timer->min = cjson_Minutes->valueint;
    timer->second = cjson_Seconds->valueint;

    cJSON_Delete(json_Handle);
}
