#pragma once
#include "CSerialPort.h"
class LaserCtrlor
{
public:
	LaserCtrlor(CSerialPort* com);

	void laser_on();
	void laser_off();
	void laser_PWM();
	void setduty(int key);
private:
	CSerialPort *comport;
	int duty;
};

