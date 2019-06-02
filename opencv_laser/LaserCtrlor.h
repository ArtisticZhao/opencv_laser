#pragma once
#include "CSerialPort.h"
class LaserCtrlor
{
public:
	LaserCtrlor(int com);
public:
	void laser_on();
	void laser_off();
private:
	CSerialPort comport;
};

