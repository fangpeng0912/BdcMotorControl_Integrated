#include "stm32f10x.h"
#include "systick_interupt_light_LED.h"
#include "RS232.h"
#include "HAL_BdcMotorControl.h"
#include "HAL_Encoder.h"
#include "APP_SpeedControl.h"

int main(){
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);    //�ж����ȼ�����,������ռ�����������ȼ�
	LED_Init();
	SYSTICK_Init(pid.Ts);                              //ÿTs ms����һ��ת�٣��������
	USART1_Init(); 
	TIM2_CH2_3_Init();                                 //���PWM�����ʱ����ʼ��
	TIM3_CH1_2_Init();                                 //������������ʱ����ʼ��
	PID_Parameters_Init(pid);
	while(1){
		//do nothing
	}
}


