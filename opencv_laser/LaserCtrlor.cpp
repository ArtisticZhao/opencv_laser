#include "LaserCtrlor.h"
#include <iostream>
using namespace std;
LaserCtrlor::LaserCtrlor(CSerialPort* com)
{
	this->comport = com;
	duty = 50;
}
void LaserCtrlor::setduty(int key)
{
	switch (key)
	{
	case 'q':
		duty = duty - 5;
		break;
	case 'e':
		duty = duty + 5;
		break;
	default:
		duty = duty;
		break;
	}
}
void LaserCtrlor::laser_on()
{
	unsigned char temp[5] = { 0xFF, 0xFF, 0xFF, 0xFF,0xFF };
	this->comport->WriteData(temp, 5);
}
void LaserCtrlor::laser_PWM(int duty)
{
	short pwm = (short)511.0 * duty / 100;
	unsigned char temp[5];
	temp[0] = 0xbb;
	temp[1] = 0xbb;
	temp[2] = 0xbb;
	temp[3] = pwm >> 8 & 0xff;
	temp[4] = pwm & 0xff;
	this->comport->WriteData(temp, 5);
}
void LaserCtrlor::laser_off()
{
	unsigned char temp[5] = { 0x00,0x00,0x00,0x00,0x00 };
	this->comport->WriteData(temp, 5);
}
