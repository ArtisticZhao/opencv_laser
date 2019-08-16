#pragma once
#include "CSerialPort.h"
class TurntableCtrl
{
public:
	TurntableCtrl(int port);
	void turn_to_deg(int angle);
	int get_angle();
	void key_to_turn(int key);
private:
	CSerialPort comport;
	int angle = 0;
};

