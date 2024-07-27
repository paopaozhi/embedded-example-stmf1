//
// Created by zhihao on 2023/11/7.
//
#include "cmsis_os2.h"
#include "led_smg.h"

void StartLedSmgTask(void *argument) {
  (void)argument;
  LED_SMG_Init();

  for (;;) {
    LED_Handle();
    osDelay(1);
  }
}