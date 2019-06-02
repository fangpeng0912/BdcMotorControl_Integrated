#include "systick_interupt_light_LED.h"

volatile double rpm = 0;

void LED_Init(void){
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin = LED_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_Init(LED_PORT, &GPIO_InitStructure);
	GPIO_ResetBits(LED_PORT, LED_PIN);
}

void SYSTICK_Init(unsigned short toggle_time){
	unsigned short fac_us, fac_ms;
	NVIC_InitTypeDef NVIC_InitStruct;
	
	if(SYSTICK_CLK_SOURCE == SysTick_CLKSource_HCLK)
		fac_us = SYSTEM_CLK_FREQ;                                  //΢�����ӣ���1us��Ҫ�������ٴ�
	else
		fac_us = SYSTEM_CLK_FREQ/8;
	fac_ms = 1000 * fac_us;
	
	SysTick_CLKSourceConfig(SYSTICK_CLK_SOURCE);                 //ʱ��Դ
	SysTick->LOAD = toggle_time * fac_ms;                        //����10ms����ΪLOAD�Ĵ���24λ��Чλ��������0xffffff
	SysTick->VAL = 0; 
	SysTick->CTRL |= 0x02;                                       //����0�����쳣���� 

	NVIC_InitStruct.NVIC_IRQChannel = (uint8_t)SysTick_IRQn;     //����NVIC��Ӧͨ���жϿ�����
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 3;       //f103�ж����ȼ����ĸ���Чλ������ԽС���ȼ�Խ��
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);

	SysTick->CTRL |= 0x01;                                       //ʹ��SysTick��ʱ��
}

void SysTick_Handler(void){
	static unsigned char light_flag = 1;
	static unsigned short i = 0;
	static long pulse_number_1 = 0, pulse_number;
	MOTOR_Output_T motor_output;
	//500ms�������߼�
	if(++i == LIGHTING_CYCLE){
		if(light_flag){
			GPIO_SetBits(LED_PORT, LED_PIN);
			light_flag = 0;
		}
		else{
			GPIO_ResetBits(LED_PORT, LED_PIN);
			light_flag = 1;
		}
		i = 0;
//		printf("ת�� = %.2frpm\r\n", rpm);
	}
	//����ʵ��ת��
	pulse_number = 65536 * overflow_count + TIM_GetCounter(TIM3);
	rpm = (double)(pulse_number - pulse_number_1) / x_PULSE_1_CYCLE /  ENCODER_LINE_NUMBER * CONVERTION2RPM;   //�������2Ŀ����Ϊ������һ�����ڶ�Ӧ�������壬����������Ϊ-374.22����λ��rpm
	pulse_number_1 = pulse_number;
	//���������ռ�ձȺͷ��򣬲����
	desired_rpm = generate_RPM();
	motor_output = BdcSpeedControl(desired_rpm);
	if(motor_output.dir == 0){
		enableBdcMotor();
		setBdcMotorFwd();
		setBdcMotorDutyCycle(motor_output.duty_cycle);
	}
	else{
		enableBdcMotor();
		setBdcMotorRev();
		setBdcMotorDutyCycle(motor_output.duty_cycle);
	}
}



