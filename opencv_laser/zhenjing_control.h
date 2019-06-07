#pragma once
#include "CSerialPort.h"
#include <cstdint>
#include "pid.h"
class ZhenjingControlor {
public:
	ZhenjingControlor(int port);
	~ZhenjingControlor(void);
	void zhenjing_control(int key);
	double get_angle_x();
	double get_angle_y();
	void set_target(int x, int y);
	void goal_target(int real_x, int real_y);
private:
	CSerialPort comport;
	Pid_control pid_x;
	Pid_control pid_y;
	double angle_x;
	double angle_y;
	int target_x=517;
	int target_y=218;
	int real_x;
	int real_y;
	int16_t ADC_DV_X;
	int16_t ADC_DV_Y;
};
