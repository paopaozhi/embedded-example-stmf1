#include "board.h"
#include "stm32f1xx_hal.h"

extern RTC_HandleTypeDef hrtc;

osStatus_t rtc_async_read(RtcTimer_TypeDef *timer)
{
    RTC_TimeTypeDef stimestructure;
    RTC_DateTypeDef sdatastructure;

    if (HAL_RTC_GetTime(&hrtc, &stimestructure, RTC_FORMAT_BIN) != HAL_OK)
    {

        board_error_handle();
    }

    if (HAL_RTC_GetDate(&hrtc, &sdatastructure, RTC_FORMAT_BIN) != HAL_OK)
    {
        board_error_handle();
    }

    timer->year = sdatastructure.Year;
    timer->month = sdatastructure.Month;
    timer->day = sdatastructure.Date;

    timer->hour = stimestructure.Hours;
    timer->min = stimestructure.Minutes;
    timer->second = stimestructure.Seconds;

    return osOK;
}

osStatus_t rtc_async_write(RtcTimer_TypeDef *timer)
{
    RTC_DateTypeDef sdatestructure;
    RTC_TimeTypeDef stimestructure;

    sdatestructure.Year = timer->year;
    sdatestructure.Month = timer->month;
    sdatestructure.Date = timer->day;
    sdatestructure.WeekDay = RTC_WEEKDAY_TUESDAY;

    if (HAL_RTC_SetDate(&hrtc, &sdatestructure, RTC_FORMAT_BIN) != HAL_OK)
    {
        return osError;
    }

    stimestructure.Hours = timer->hour;
    stimestructure.Minutes = timer->min;
    stimestructure.Seconds = timer->second;

    if (HAL_RTC_SetTime(&hrtc, &stimestructure, RTC_FORMAT_BIN) != HAL_OK)
    {
        return osError;
    }

    return osOK;
}
