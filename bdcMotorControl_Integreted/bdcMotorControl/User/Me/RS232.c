#include "RS232.h"

static unsigned char USART1_receive;
volatile double desired_rpm = 0;

void USART1_Init(){
	GPIO_InitTypeDef GPIO_InitStructure_Tx, GPIO_InitStructure_Rx;
	USART_InitTypeDef USART_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;

	//初始化GPIOA相关PIN脚和USART1
	GPIO_InitStructure_Tx.GPIO_Pin = USART1_TX_PIN;	
	GPIO_InitStructure_Tx.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure_Tx.GPIO_Mode = GPIO_Mode_AF_PP;

	GPIO_InitStructure_Rx.GPIO_Pin = USART1_RX_PIN;	
	GPIO_InitStructure_Rx.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure_Rx.GPIO_Mode = GPIO_Mode_IPU;

	USART_InitStruct.USART_BaudRate = 9600;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_Init(USART1_PORT, &GPIO_InitStructure_Tx);
	GPIO_Init(USART1_PORT, &GPIO_InitStructure_Rx);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	USART_Init(USART1, &USART_InitStruct);         
	//使能USART1接收中断
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	
	//配置中断优先级以及使能相关中断通道
	NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);

	//使能USART1
	USART_Cmd(USART1, ENABLE);
}

void USART1_IRQHandler(void){
	if(USART_GetITStatus(USART1, USART_IT_RXNE) == SET){ 
		//接收串口数据
		USART1_receive = USART_ReceiveData(USART1);
		desired_rpm = (double)USART1_receive;
		//清零接收中断标志位
		USART_ClearFlag(USART1, USART_FLAG_RXNE);
	}
}

int fputc(int ch, FILE *file){ 
	USART_GetFlagStatus(USART1, USART_FLAG_TC);  
	USART_SendData(USART1, (uint8_t)ch);
	while(!USART_GetFlagStatus(USART1, USART_FLAG_TC));
	
	return ch;
}


