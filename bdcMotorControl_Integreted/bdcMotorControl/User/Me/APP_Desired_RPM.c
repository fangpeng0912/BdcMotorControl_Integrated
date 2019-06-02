#include "APP_Desired_RPM.h"

double generate_RPM(void){
	static int t = 0;
	double ideal_rpm;
	ideal_rpm = 60 * PI * sin(12 * PI * ((double)t / 60000));    //10s: 0r->10r->0r
	++t;
	if(t == 10000){
		t = 0;
	}
	return ideal_rpm;
}
