#ifndef _HAL_ENCODER_H_
#define _HAL_ENCODER_H_

#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_tim.h"
#include "misc.h"
#include "RS232.h"

#define ENCODER_PORT GPIOA
#define A_PHASE_PIN GPIO_Pin_6
#define B_PHASE_PIN GPIO_Pin_7
#define ENCODER_LINE_NUMBER (-510.3)
#define x_PULSE_1_CYCLE 2

extern volatile int overflow_count;

void TIM3_CH1_2_Init(void);

#endif
