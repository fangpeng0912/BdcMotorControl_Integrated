#include "HAL_Encoder.h"

volatile int overflow_count = 0;

void TIM3_CH1_2_Init(void){
	GPIO_InitTypeDef GPIO_InitStructure_ENCODER;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure_A_PHASE, TIM_ICInitStructure_B_PHASE;
	NVIC_InitTypeDef NVIC_InitStructure;

	GPIO_InitStructure_ENCODER.GPIO_Pin = A_PHASE_PIN | B_PHASE_PIN;
	GPIO_InitStructure_ENCODER.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure_ENCODER.GPIO_Mode = GPIO_Mode_IN_FLOATING;

	TIM_TimeBaseInitStructure.TIM_Period = 0xffff;
	TIM_TimeBaseInitStructure.TIM_Prescaler = 0; 
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_ICInitStructure_A_PHASE.TIM_Channel = TIM_Channel_1;                     //ͨ��1
	TIM_ICInitStructure_A_PHASE.TIM_ICFilter = 0x00;                             //�˲�
	TIM_ICInitStructure_A_PHASE.TIM_ICPolarity = TIM_ICPolarity_Rising;          //�����ԣ�������ȡ�����˲�ʱ��TI1��TI1FP1�ź��෴
	TIM_ICInitStructure_A_PHASE.TIM_ICPrescaler = TIM_ICPSC_DIV1;                //��Ƶϵ��
	TIM_ICInitStructure_A_PHASE.TIM_ICSelection = TIM_ICSelection_DirectTI;      //ֱ��ӳ�䵽TI1

	TIM_ICInitStructure_B_PHASE.TIM_Channel = TIM_Channel_2;                     //ͨ��2
	TIM_ICInitStructure_B_PHASE.TIM_ICFilter = 0x00;                             
	TIM_ICInitStructure_B_PHASE.TIM_ICPolarity = TIM_ICPolarity_Rising;         
	TIM_ICInitStructure_B_PHASE.TIM_ICPrescaler = TIM_ICPSC_DIV1;               
	TIM_ICInitStructure_B_PHASE.TIM_ICSelection = TIM_ICSelection_DirectTI; 

	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;                              //�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;                    //��ռ���ȼ�
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;                           //�����ȼ�
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                              //IRQͨ��ʹ��

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_Init(ENCODER_PORT, &GPIO_InitStructure_ENCODER);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);
	TIM_ICInit(TIM3, &TIM_ICInitStructure_A_PHASE);
	TIM_ICInit(TIM3, &TIM_ICInitStructure_B_PHASE);                     
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
	TIM_ClearITPendingBit(TIM3, TIM_IT_Update);	
	
	TIM3->SMCR &= (uint16_t)(~((uint16_t)TIM_SMCR_SMS));                         //������ģʽ2������TI2FP2�ĵ�ƽ����������TI1FP1�ı������¼���
	TIM3->SMCR |= TIM_EncoderMode_TI2;

	NVIC_Init(&NVIC_InitStructure);

	TIM_Cmd(TIM3, ENABLE);                                                       //ʹ�ܶ�ʱ��
}

//������ģʽ�������¾���������ж�
void TIM3_IRQHandler(void){        
	if(TIM_GetITStatus(TIM3, TIM_IT_Update) == SET){
		if((TIM3->CR1 &  0x10) == 0){                                              //��������ж�
			++overflow_count;
		}
		else{
			--overflow_count;
		}
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);	
	}
}



