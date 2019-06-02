#pragma once
#include "CSerialPort.h"
class ZhenjingControlor {
public:
	ZhenjingControlor(int port);
	~ZhenjingControlor(void);
	void zhenjing_control(int key);
private:
	CSerialPort comport;
};
