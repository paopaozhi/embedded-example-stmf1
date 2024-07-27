#include "board.h"
#include "stm32f1xx_hal.h"

extern RTC_HandleTypeDef hrtc;

osStatus_t rtc_async_read(RtcTimer_TypeDef *timer)
{
    RTC_TimeTypeDef stimestructure;
    RTC_DateTypeDef sdatastructure;

    if (HAL_RTC_GetTime(&hrtc, &stimestructure, RTC_FORMAT_BCD) != HAL_OK)
    {

        board_error_handle();
    }

    if(HAL_RTC_GetDate(&hrtc,&sdatastructure,RTC_FORMAT_BCD) == HAL_OK)
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

    /*##-1- Configure the Date #################################################*/
    /* Set Date: Tuesday February 18th 2014 */
    sdatestructure.Year = 0x14;
    sdatestructure.Month = RTC_MONTH_FEBRUARY;
    sdatestructure.Date = 0x18;
    sdatestructure.WeekDay = RTC_WEEKDAY_TUESDAY;

    if (HAL_RTC_SetDate(&hrtc, &sdatestructure, RTC_FORMAT_BCD) != HAL_OK)
    {
        /* Initialization Error */
        return osError;
    }

    /*##-2- Configure the Time #################################################*/
    /* Set Time: 02:20:00 */
    stimestructure.Hours = 0x02;
    stimestructure.Minutes = 0x20;
    stimestructure.Seconds = 0x00;

    if (HAL_RTC_SetTime(&hrtc, &stimestructure, RTC_FORMAT_BCD) != HAL_OK)
    {
        /* Initialization Error */
        return osError;
    }
}
