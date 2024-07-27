//
// Created by zhihao on 2023/11/7.
//
#include "cmsis_os2.h"
#include "led_smg.h"

void StartLedSmgTask(void *argument) {
  (void)argument;
  LED_SMG_Init();

  osDelay(10);
  LED_Str("123");
  for (;;) {
    LED_Handle();
    osDelay(1);
  }
}