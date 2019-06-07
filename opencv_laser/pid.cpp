#include "pid.h"
#include <stdio.h>
#include <iostream>

using namespace std;
void Pid_control::PID_init(float kp, float ki, float kd)
{
	printf("PID_init begin \n");
	pid.target = 515.0;
	pid.Actual = 0.0;
	pid.err = 0.0;
	pid.err_last = 0.0;
	pid.step = 0.0;
	pid.integral = 0.0;
	pid.Kp = kp;
	pid.Ki = ki;
	pid.Kd = kd;
	printf("PID_init end \n");
}

float Pid_control::PID_realize(float end, float real)
{
	cout << end << " " << endl;
	if (end - real<3 && end - real>-3)
	{
		cout << "ok" << endl;
		return 0;
	}
	pid.target = end;
	pid.err = pid.target - pid.Actual;
	pid.integral += pid.err;
	pid.step = pid.Kp * pid.err + pid.Ki * pid.integral + pid.Kd * (pid.err - pid.err_last);
	pid.err_last = pid.err;
	pid.Actual = real;
	return pid.step;
}

