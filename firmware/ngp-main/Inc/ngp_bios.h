#ifndef __NGP_BIOS_H_
#define __NGP_BIOS_H_


#include "stm32f4xx_hal.h"

void _tim_irq_callback(TIM_HandleTypeDef *htim);
void _bi_led_tick();

void setup();
void loop();


#endif
