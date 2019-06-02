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

//����PID��������ʼ��
void PID_Parameters_Init(PID pid);
//���������ռ�ձȺͷ���
MOTOR_Output_T BdcSpeedControl(double desired_rpm);

#endif
