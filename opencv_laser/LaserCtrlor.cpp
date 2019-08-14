#include "LaserCtrlor.h"
#include <iostream>
#include <opencv.hpp>
using namespace std;
LaserCtrlor::LaserCtrlor(CSerialPort* com)
{
	this->comport = com;
	duty = 3.7;
	this->laser_PWM();
}
void LaserCtrlor::setduty(int key)
{
	switch (key)
	{
	case 'r':
		duty = duty - 0.1;
		if (duty < 0) {
			duty = 100;
		}
		cout << "laser duty: " << duty << endl;
		laser_PWM();
		break;
	case 'e':
		duty = duty + 0.1;
		if (duty > 100) {
			duty = 0;
		}
		cout << "laser duty: " << duty << endl;
		laser_PWM();
		break;
	default:
		duty = duty;
		break;
	}
}
void LaserCtrlor::set_duty(double duty)
{
	this->duty = duty;
	cout << "laser duty: " << duty << endl;
	laser_PWM();
}
void LaserCtrlor::laser_on()
{
	unsigned char temp[5] = { 0xFF, 0xdc, 0xdc, 0xdc,0xdc };
	this->comport->WriteData(temp, 5);
}
void LaserCtrlor::laser_PWM()
{
	short pwm = (short)511.0 * duty / 100;
	unsigned char temp[5];
	temp[0] = 0xbb;
	temp[1] = 0xbb;
	temp[2] = 0xbb;
	temp[3] = pwm >> 8 & 0xff;
	temp[4] = pwm & 0xff;
	cv::waitKey(100);
	this->comport->WriteData(temp, 5);
	cv::waitKey(100);
}
void LaserCtrlor::laser_off()
{
	unsigned char temp[5] = { 0x00,0xdc,0xdc,0xdc,0xdc };
	this->comport->WriteData(temp, 5);
}
