#include "APP_SpeedControl.h"

const PID pid = {0.2, 0, 0, 1};                    //Kp, Ki, Kd, Ts(单位ms)
static double A, B, C;

void PID_Parameters_Init(PID pid){
	A = pid.Kp + pid.Ki * ((double)pid.Ts / 60000) + pid.Kd / ((double)pid.Ts / 60000);     //注意：这里Ts要除以60000，换算为min，因为转速单位为r/min
	B = -pid.Kp - 2 * pid.Kd / ((double)pid.Ts / 60000);
	C = pid.Kd / ((double)pid.Ts / 60000);
}

MOTOR_Output_T BdcSpeedControl(double desired_rpm){
	static double e_2 = 0, e_1 = 0, u_1 = 0;
	double e, delta_u, u;
	MOTOR_Output_T motor_output;
	//计算输出电压
	e = desired_rpm - rpm;
	delta_u = A * e + B * e_1 + C * e_2;
	u = u_1 + delta_u;
	//设置占空比
	if(u >= 0 && u <= BDC_RATED_VOLTAGE){
		motor_output.duty_cycle = (uint8_t)(ABS(u) / BDC_RATED_VOLTAGE * 100);    //电机占空比
		motor_output.dir = 0;                                                     //逆时针
	}
	else if(u >= -BDC_RATED_VOLTAGE && u < 0){
		motor_output.duty_cycle = (uint8_t)(ABS(u) / BDC_RATED_VOLTAGE * 100);    //电机占空比
		motor_output.dir = 1;                                                     //顺时针
	}
	else if(u > BDC_RATED_VOLTAGE){
		motor_output.duty_cycle = 100;                                            //电机满占空比
		motor_output.dir = 0;                                                     //逆时针
	}
	else{
		motor_output.duty_cycle = 100;                                            //电机满占空比
		motor_output.dir = 1;                                                     //顺时针
	}
	//保存旧值
	e_2 = e_1;
	e_1 = e;
	u_1 = u;
	return motor_output;
}
