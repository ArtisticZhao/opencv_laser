#include "TurntableCtrl.h"
#include <iostream>
#include <stdio.h>
#include <opencv.hpp>

TurntableCtrl::TurntableCtrl(int port)
{
	bool res;
	res = this->comport.InitPort(port, CBR_9600);
	if (!res) {
		std::cout << "转台串口初始化失败!" << res << std::endl;
	}
	// 初始化转台
	unsigned char temp[] = "s r0x24 21\r\n"; 
	this->comport.WriteData(temp, 12);
	unsigned char temp1[] = "s r0xc8 0\r\n";
	this->comport.WriteData(temp1, 11);
	unsigned char temp2[] = "s r0xca 0\r\n";
	this->comport.WriteData(temp2, 11);
	unsigned char temp3[] = "t 1\r\n";
	this->comport.WriteData(temp3, 5);
	cv::waitKey(6000);
}

void TurntableCtrl::turn_to_deg(int angle)
{
	char temp2[20];
	unsigned char temp3[] = "t 1\r\n";
	switch (angle)
	{
	case 0:
		this->angle = angle;
		sprintf_s(temp2, "s r0xca 0\r\n");
		this->comport.WriteData((unsigned char*)temp2, 11);
		this->comport.WriteData(temp3, 5);
		cv::waitKey(2000);
		break;
	case -90:
		this->angle = angle;
		sprintf_s(temp2, "s r0xca -301440\r\n");
		this->comport.WriteData((unsigned char*)temp2, 17);
		this->comport.WriteData(temp3, 5);
		cv::waitKey(2000);
		break;
	case -180:
		this->angle = angle;
		sprintf_s(temp2, "s r0xca -602880\r\n");
		this->comport.WriteData((unsigned char*)temp2, 17);
		this->comport.WriteData(temp3, 5);
		cv::waitKey(2000);
		break;
	case -270:
		this->angle = angle;
		sprintf_s(temp2, "s r0xca -904320\r\n");
		this->comport.WriteData((unsigned char*)temp2, 17);
		this->comport.WriteData(temp3, 5);
		cv::waitKey(2000);
		cv::waitKey(2000);
		break;
	case -360:
		this->angle = angle;
		sprintf_s(temp2, "s r0xca -1205760\r\n");
		this->comport.WriteData((unsigned char*)temp2, 18);
		this->comport.WriteData(temp3, 5);
		break;
	default:
		break;
	}
	std::cout << "turn to " << angle << std::endl;
}

int TurntableCtrl::get_angle()
{
	return this->angle;
}

void TurntableCtrl::key_to_turn(int key)
{
	switch (key)
	{
	case '6':
		this->turn_to_deg(0);
		break;
	case '7':
		this->turn_to_deg(-90);
		break;
	case '8':
		this->turn_to_deg(-180);
		break;
	case '9':
		this->turn_to_deg(-270);
		break;
	case '0':
		this->turn_to_deg(-360);
		break;
	default:
		break;
	}
}
