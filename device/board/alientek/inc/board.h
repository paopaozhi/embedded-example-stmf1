#ifndef __BOARD_H
#define __BOARD_H

#include <stdint.h>
#include "cmsis_os2.h"

typedef enum
{
    uart1 = 0,
    uart2,
    uart3,
    uart4
} UartId_TypeDef;

typedef struct
{
    uint32_t year;
    uint8_t month;
    uint8_t day;

    uint8_t hour;
    uint8_t min;
    uint8_t second;
} RtcTimer_TypeDef;

typedef int32_t (*RegisterAlarmCallback)(void);

// uart
void uart_init(UartId_TypeDef uart);
void uart_read(UartId_TypeDef uart);
osStatus_t uart_async_read(UartId_TypeDef uart, uint8_t *buf, uint32_t len);
void uart_write(UartId_TypeDef uart);
osStatus_t uart_async_write(UartId_TypeDef uart, uint8_t *buf, uint32_t len);
void uart_get_state(UartId_TypeDef uart);

// rtc
osStatus_t rtc_async_read(RtcTimer_TypeDef *timer);
osStatus_t rtc_async_write(RtcTimer_TypeDef *timer);
void rtc_config_alrm_callback(RegisterAlarmCallback alrm_callback);

// adc
void adc_init(void);
void adc_read(void);
void adc_async_read(void);

// pwm
void pwm_init(void);

// iic
void iic_init(void);
void iic_read(void);
void iic_async_read(void);
void iic_write(void);
void iic_async_write(void);

// private

void board_error_handle(void);

#endif
