#pragma once
#include "CSerialPort.h"
class ZhenjingControlor {
public:
	ZhenjingControlor(int port);
	~ZhenjingControlor(void);
	void zhenjing_control(int key);
	double get_angle_x();
	double get_angle_y();
private:
	CSerialPort comport;
	double angle_x;
	double angle_y;
};
