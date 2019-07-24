#include "zhenjing_control.h"
#include <cstdint>
#include <iostream>
#include <opencv.hpp>
//#include <iostream>
using namespace std;
void ZhenjingControlor::zhenjing_control(int key)
{
	static int16_t step;
	bool show_flag = true;
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
	case '5':
		step = 5000;
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
		show_flag = false;
		ADC_DV_X = ADC_DV_X;
		ADC_DV_Y = ADC_DV_Y;
		break;
	}
	if (show_flag) {  // 当只有移动光点的时候才显示回显
		//cout << "x: " << ADC_DV_X << "y: " << ADC_DV_Y << endl;
		unsigned char temp[5];
		temp[3] = ADC_DV_X >> 8 & 0xff;
		temp[4] = ADC_DV_X & 0xff;

		temp[1] = ADC_DV_Y >> 8 & 0xff;
		temp[2] = ADC_DV_Y & 0xff;
		temp[0] = 0xaa;
		// send comport
		//printf("key: %x %x %x %x\n", temp[3], temp[2], temp[1], temp[0]);
		cv::waitKey(100);
		this->comport.WriteData(temp, 5);
		cv::waitKey(100);
		// update angle
		this->angle_x = 10.0 / 32768 * ADC_DV_X / 0.8;
		this->angle_y = 10.0 / 32768 * ADC_DV_Y / 0.8;
	}	
	
}

void ZhenjingControlor::goto_volt(int x, int y)
{
	ADC_DV_X = x;
	ADC_DV_Y = y;
	unsigned char temp[5];
	temp[3] = ADC_DV_X >> 8 & 0xff;
	temp[4] = ADC_DV_X & 0xff;

	temp[1] = ADC_DV_Y >> 8 & 0xff;
	temp[2] = ADC_DV_Y & 0xff;
	temp[0] = 0xaa;
	// send comport
	//printf("key: %x %x %x %x\n", temp[3], temp[2], temp[1], temp[0]);
	cv::waitKey(100);
	this->comport.WriteData(temp, 5);
	cv::waitKey(100);
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

void ZhenjingControlor::goal_target(int real_x, int real_y, bool is_pid_x, bool is_pid_y, bool has_point)
{

	this->real_x = real_x;
	this->real_y = real_y;
	int step_x = 0;
	int step_y = 0;
	if (is_pid_x)
		step_x = pid_x.PID_realize(target_x, real_x);
	if (is_pid_y)
		step_y = pid_y.PID_realize(target_y, real_y);
	int ctrl_val = ADC_DV_X;
	int ctrl_val_y = ADC_DV_Y;
	if (has_point) {
		ctrl_val = ADC_DV_X + step_x;
		ctrl_val_y = ADC_DV_Y + step_y;
	}
	else {
		if (step_x > 0) {
			ctrl_val += STEP;
		}
		else if (step_x < 0) {
			ctrl_val -= STEP;
		}
		if (step_y > 0) {
			ctrl_val_y += STEP;
		}
		else if (step_y < 0) {
			ctrl_val_y -= STEP;
		}
	}
	
	// 对其进行限幅
	if (step_x > 0 && ctrl_val > 32767) {
		ADC_DV_X = 32760;
	}
	else if (step_x < 0 && ctrl_val < -32768) {
		ADC_DV_X = -32760;
	}
	else {
		ADC_DV_X = ctrl_val;
	}
	if (step_y > 0 && ctrl_val_y > 32767) {
		ADC_DV_Y = 32767;
	}
	else if (step_y < 0 && ctrl_val_y < -32768) {
		ADC_DV_Y = -32767;
	}
	else {
		ADC_DV_Y = ctrl_val_y;
	}
	unsigned char temp[5];
	temp[3] = ADC_DV_X >> 8 & 0xff;
	temp[4] = ADC_DV_X & 0xff;

	temp[1] = ADC_DV_Y >> 8 & 0xff;
	temp[2] = ADC_DV_Y & 0xff;
	temp[0] = 0xaa;
	// send comport
	//printf("goal: %x %x %x %x\n", temp[3], temp[2], temp[1], temp[0]);
	if (is_pid_x || is_pid_y) {
		cv::waitKey(100);
		this->comport.WriteData(temp, 5);
		// update angle
		this->angle_x = 10.0 / 32768 * ADC_DV_X / 0.8;
		this->angle_y = 10.0 / 32768 * ADC_DV_Y / 0.8;
	}
}

void ZhenjingControlor::show_volts()
{
	cout << "Volt: x: " << this->ADC_DV_X << " y: " << this->ADC_DV_Y << endl;
}

CSerialPort* ZhenjingControlor::get_serial_port()
{
	return &this->comport;
}
	
ZhenjingControlor::ZhenjingControlor(int port)
{
	pid_x.PID_init(30, 1, 0.25, 'x');
	pid_y.PID_init(30, 1, 0.25, 'y');
	bool res;
	res = this->comport.InitPort(port, CBR_115200);
	if (!res) {
		cout << "振镜串口初始化失败!" << res << endl;
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
