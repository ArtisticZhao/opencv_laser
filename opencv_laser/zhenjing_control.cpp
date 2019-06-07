#include "zhenjing_control.h"
#include <cstdint>
#include <iostream>
using namespace std;
void ZhenjingControlor::zhenjing_control(int key)
{
	static int16_t step;
	switch (key)
	{
	case '1':
		step = 10;
		break;
	case '2':
		step = 100;
		break;
	case '3':
		step = 1000;
		break;
	case '4':
		step = 10000;
		break;
	case 'w':
		ADC_DV_Y = ADC_DV_Y - step;
		break;
	case 's':
		ADC_DV_Y = ADC_DV_Y + step;
		break;
	case 'a':
		ADC_DV_X = ADC_DV_X - step;
		break;
	case 'd':
		ADC_DV_X = ADC_DV_X + step;
		break;
	default:
		ADC_DV_X = ADC_DV_X;
		ADC_DV_Y = ADC_DV_Y;
		break;
	}
	cout <<"x: "<< ADC_DV_X << "y: " << ADC_DV_Y << endl;
	unsigned char temp[4];
	temp[2] = ADC_DV_X >> 8 & 0xff;
	temp[3] = ADC_DV_X & 0xff;

	temp[0] = ADC_DV_Y >> 8 & 0xff;
	temp[1] = ADC_DV_Y & 0xff;
	// send comport
	//printf("key: %x %x %x %x\n", temp[3], temp[2], temp[1], temp[0]);
	this->comport.WriteData(temp, 4);
	// update angle
	this->angle_x = 10.0 / 32768 * ADC_DV_X / 0.8;
	this->angle_y = 10.0 / 32768 * ADC_DV_Y / 0.8;
}

double ZhenjingControlor::get_angle_x()
{
	//cout << "angle X: " << 90 + 2 * this->angle_x << endl;
	return (90 + 2*this->angle_x);
}

double ZhenjingControlor::get_angle_y()
{
	//cout << "angle Y: " << 2*this->angle_y << endl;
	return 2*this->angle_y;
}

void ZhenjingControlor::set_target(int x, int y)
{
	target_x = x;
	target_y = y;
}

void ZhenjingControlor::goal_target(int real_x, int real_y)
{

	this->real_x = real_x;
	this->real_y = real_y;
	ADC_DV_X = ADC_DV_X + pid_x.PID_realize(target_x, real_x);
	ADC_DV_Y = ADC_DV_Y + pid_y.PID_realize(target_y, real_y);
	unsigned char temp[4];
	temp[2] = ADC_DV_X >> 8 & 0xff;
	temp[3] = ADC_DV_X & 0xff;

	temp[0] = ADC_DV_Y >> 8 & 0xff;
	temp[1] = ADC_DV_Y & 0xff;
	// send comport
	//printf("goal: %x %x %x %x\n", temp[3], temp[2], temp[1], temp[0]);
	this->comport.WriteData(temp, 4);
	// update angle
	this->angle_x = 10.0 / 32768 * ADC_DV_X / 0.8;
	this->angle_y = 10.0 / 32768 * ADC_DV_Y / 0.8;
}
	
ZhenjingControlor::ZhenjingControlor(int port)
{
	pid_x.PID_init(15, 0, 0);
	pid_y.PID_init(15, 0, 0);
	bool res;
	res = this->comport.InitPort(port, CBR_115200);
	if (!res) {
		cout << "Õñ¾µ´®¿Ú³õÊ¼»¯Ê§°Ü!" << res << endl;
	}
	this->angle_x = 0;
	this->angle_y = 0;
	this->ADC_DV_X = 0;
	this->ADC_DV_Y = 0;
	this->real_x = 0;
	this->real_y = 0;
}

ZhenjingControlor::~ZhenjingControlor(void)
{
	comport.~CSerialPort();
}
