#ifndef _HAL_BDCMOTOR_CONTROL_H_
#define _HAL_BDCMOTOR_CONTROL_H_

#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_tim.h"

#define INx_PORT GPIOA
#define IN1_PIN GPIO_Pin_1
#define IN2_PIN GPIO_Pin_2

#define TIM2_CLK_FREQ 72000000
#define PSC (72-1)
#define CNT_CLK_FREQ (TIM2_CLK_FREQ/(PSC+1))
#define freq2arr(freq) (CNT_CLK_FREQ/(freq)-1)
#define dutyCycle2ccr(duty_cycle) ((float)duty_cycle/100*(HAL_BdcMotor_Freq+1))

typedef enum{
	FALSE,
	TRUE
}Boolean;

void TIM2_CH2_3_Init(void);
static void IOsetDutyCycle(uint16_t duty_cycle);

//无刷直流电机驱动
void disableBdcMotor(void);
void enableBdcMotor(void);
void setBdcMotorFwd(void);
void setBdcMotorRev(void);
void setBdcMotorFreq(uint16_t freq);
void setBdcMotorDutyCycle(uint8_t duty_cycle);

#endif
