#ifndef _APP_SPEED_CONTROL_
#define _APP_SPEED_CONTROL_

#include "stm32f10x.h"
#include "systick_interupt_light_LED.h"

#define BDC_RATED_VOLTAGE 12
#define ABS(u) ((u) >= 0 ? u : -u)

typedef struct{
	double Kp;
	double Ki;
	double Kd;
	uint16_t Ts;
}PID;

typedef struct{
	uint8_t duty_cycle;
	uint8_t dir;
}MOTOR_Output_T;

extern const PID pid;

//增量PID各参数初始化
void PID_Parameters_Init(PID pid);
//计算电机输出占空比和方向
MOTOR_Output_T BdcSpeedControl(double desired_rpm);

#endif
