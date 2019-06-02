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
		fac_us = SYSTEM_CLK_FREQ;                                  //微秒因子，即1us需要计数多少次
	else
		fac_us = SYSTEM_CLK_FREQ/8;
	fac_ms = 1000 * fac_us;
	
	SysTick_CLKSourceConfig(SYSTICK_CLK_SOURCE);                 //时钟源
	SysTick->LOAD = toggle_time * fac_ms;                        //计数10ms，因为LOAD寄存器24位有效位，因此最大0xffffff
	SysTick->VAL = 0; 
	SysTick->CTRL |= 0x02;                                       //数到0产生异常请求 

	NVIC_InitStruct.NVIC_IRQChannel = (uint8_t)SysTick_IRQn;     //设置NVIC对应通道中断控制器
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 3;       //f103中断优先级共四个有效位，数字越小优先级越高
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);

	SysTick->CTRL |= 0x01;                                       //使能SysTick定时器
}

void SysTick_Handler(void){
	static unsigned char light_flag = 1;
	static unsigned short i = 0;
	static long pulse_number_1 = 0, pulse_number;
	MOTOR_Output_T motor_output;
	//500ms点灯灭灯逻辑
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
//		printf("转速 = %.2frpm\r\n", rpm);
	}
	//计算实际转速
	pulse_number = 65536 * overflow_count + TIM_GetCounter(TIM3);
	rpm = (double)(pulse_number - pulse_number_1) / x_PULSE_1_CYCLE /  ENCODER_LINE_NUMBER * CONVERTION2RPM;   //这里除以2目的因为编码器一个周期对应两个脉冲，编码器线数为-374.22，单位：rpm
	pulse_number_1 = pulse_number;
	//计算电机输出占空比和方向，并输出
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



