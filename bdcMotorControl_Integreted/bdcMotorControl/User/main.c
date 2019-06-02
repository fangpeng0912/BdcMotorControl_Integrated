#include "stm32f10x.h"
#include "systick_interupt_light_LED.h"
#include "RS232.h"
#include "HAL_BdcMotorControl.h"
#include "HAL_Encoder.h"
#include "APP_SpeedControl.h"

int main(){
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);    //中断优先级分组,两级抢占，两级亚优先级
	LED_Init();
	SYSTICK_Init(pid.Ts);                              //每Ts ms计算一次转速，并且输出
	USART1_Init(); 
	TIM2_CH2_3_Init();                                 //电机PWM输出定时器初始化
	TIM3_CH1_2_Init();                                 //编码器采样定时器初始化
	PID_Parameters_Init(pid);
	while(1){
		//do nothing
	}
}


