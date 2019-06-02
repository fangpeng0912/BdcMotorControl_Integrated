#ifndef _SYSTICK_INTERUPT_LIGHT_LED_
#define _SYSTICK_INTERUPT_LIGHT_LED_

#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "misc.h"
#include "stm32f10x_tim.h"
#include "RS232.h"
#include "HAL_Encoder.h"
#include "HAL_BdcMotorControl.h"
#include "APP_SpeedControl.h"
#include "APP_Desired_RPM.h"

#define SYSTEM_CLK_FREQ 72
#define SYSTICK_CLK_SOURCE SysTick_CLKSource_HCLK_Div8
#define LED_PORT GPIOA
#define LED_PIN GPIO_Pin_0
#define LIGHTING_CYCLE (500 / pid.Ts)
#define CONVERTION2RPM (60000 / pid.Ts)

extern volatile double rpm;

void LED_Init(void);
void SYSTICK_Init(unsigned short toggle_time);

#endif
