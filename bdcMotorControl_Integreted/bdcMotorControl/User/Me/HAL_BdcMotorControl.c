#include "HAL_BdcMotorControl.h"

static uint8_t HAL_BdcMotor_Is_Enabled = FALSE;
static uint8_t HAL_BdcMotor_Fwd_CTL = TRUE;
static uint16_t HAL_BdcMotor_Freq = freq2arr(2000); //default: 2000HZ
static uint16_t HAL_BdcMotor_Duty_Cycle = 0; //default: 0

void TIM2_CH2_3_Init(void){
	GPIO_InitTypeDef GPIO_InitStructure_INx;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure_CHx;
	
	GPIO_InitStructure_INx.GPIO_Pin = IN1_PIN | IN2_PIN;
	GPIO_InitStructure_INx.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure_INx.GPIO_Mode = GPIO_Mode_AF_PP;
	
	TIM_TimeBaseInitStructure.TIM_Period = HAL_BdcMotor_Freq;
	TIM_TimeBaseInitStructure.TIM_Prescaler = PSC;  
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	
	TIM_OCInitStructure_CHx.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure_CHx.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure_CHx.TIM_OutputState=  TIM_OutputState_Enable;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_Init(INx_PORT, &GPIO_InitStructure_INx);
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
	
	TIM_OC2Init(TIM2, &TIM_OCInitStructure_CHx);
	TIM_OC3Init(TIM2, &TIM_OCInitStructure_CHx);

	TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable); 
	TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable); 
	TIM_ARRPreloadConfig(TIM2, ENABLE);
	
	//输出占空比初始为0
	TIM_SetCompare2(TIM2, HAL_BdcMotor_Duty_Cycle);
	TIM_SetCompare3(TIM2, HAL_BdcMotor_Duty_Cycle);
	
	//使能TIM3
	TIM_Cmd(TIM2, ENABLE); 
}

void IOsetDutyCycle(uint16_t duty_cycle){
	if(HAL_BdcMotor_Fwd_CTL == TRUE){
		TIM_SetCompare2(TIM2, duty_cycle);
		TIM_SetCompare3(TIM2, 0);
	}
	else{
		TIM_SetCompare2(TIM2, 0);
		TIM_SetCompare3(TIM2, duty_cycle);
	}
}

/* 失能
   .   .
   .   .   
   .....      
*/
void disableBdcMotor(void){
	if(HAL_BdcMotor_Is_Enabled){
		setBdcMotorDutyCycle(0);
	}
	HAL_BdcMotor_Is_Enabled = FALSE;
}

//使能
void enableBdcMotor(void){
	if(!HAL_BdcMotor_Is_Enabled){
		HAL_BdcMotor_Is_Enabled = TRUE;
	}		
}

/* 逆时针
       .
       .   
   .....
   .
   .      
*/
void setBdcMotorFwd(void){                                        
	if(HAL_BdcMotor_Is_Enabled){                                   
		TIM_SetCompare2(TIM2, HAL_BdcMotor_Duty_Cycle);             
		TIM_SetCompare3(TIM2, 0);                                 
	}
	HAL_BdcMotor_Fwd_CTL = TRUE;
}

/* 顺时针
   .   
   .      
   .....
       .
       . 
*/
void setBdcMotorRev(void){                                
	if(HAL_BdcMotor_Is_Enabled){
		TIM_SetCompare2(TIM2, 0);
		TIM_SetCompare3(TIM2, HAL_BdcMotor_Duty_Cycle);
	}
	HAL_BdcMotor_Fwd_CTL = FALSE;
}

//设置方波频率，由于此时占空比没变化，因此比较寄存器需跟着变
void setBdcMotorFreq(uint16_t freq){
	if(HAL_BdcMotor_Is_Enabled){
		if(freq < 1000){
			freq = 1000;
		}
		HAL_BdcMotor_Freq = freq2arr(freq);
		TIM_SetAutoreload(TIM2, HAL_BdcMotor_Freq);
		
		IOsetDutyCycle(HAL_BdcMotor_Duty_Cycle);
	}
}

//设置电机占空比
void setBdcMotorDutyCycle(uint8_t duty_cycle){
	if(HAL_BdcMotor_Is_Enabled){
		if(duty_cycle > 100){
			duty_cycle = 100 ;
		}
		HAL_BdcMotor_Duty_Cycle = dutyCycle2ccr(duty_cycle);
		IOsetDutyCycle(HAL_BdcMotor_Duty_Cycle);
	}
}

