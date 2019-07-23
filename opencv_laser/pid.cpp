#include "pid.h"
#include <stdio.h>
#include <iostream>
extern bool is_pid;
extern bool is_pid_y;
using namespace std;
void Pid_control::PID_init(float kp, float ki, float kd, char name)
{
	this->kp = kp;
	this->name = name;
	pid.target = 0.0;
	pid.Actual = 0.0;
	pid.err = 0.0;
	pid.err_last = 0.0;
	pid.step = 0.0;
	pid.integral = 0.0;
	pid.Kp = kp;
	pid.Ki = ki;
	pid.Kd = kd;
}

float Pid_control::PID_realize(float end, float real)
{
	cout << "real: " << real << "  end: " << end << endl;
	cout << end << " " << endl;
	if (end - real<3 && end - real>-3)
	{
		cout << "ok" << endl;
		this->pid.Kp = this->kp;
		if (name == 'x') {
			is_pid = false;
		}
		else {
			is_pid_y = false;
		}
		
		return 0;
	}
	if (end - real<30 && end - real>-30)
	{
		count++;
		if (count > 5) {
			this->pid.Kp = 60;
		}
	}
	pid.target = end;
	pid.Actual = real;
	pid.err = pid.target - pid.Actual;
	cout << "err: " << pid.err << endl;
	pid.integral += pid.err;
	pid.step = pid.Kp * pid.err + pid.Ki * pid.integral + pid.Kd * (pid.err - pid.err_last);
	pid.err_last = pid.err;
	

	cout << pid.step << endl;
	return pid.step;
}

