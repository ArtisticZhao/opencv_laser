#include "LaserCtrlor.h"
#include <iostream>
using namespace std;
LaserCtrlor::LaserCtrlor(int com)
{
	bool res;
	res = this->comport.InitPort(com, CBR_115200);
	if (!res) {
		cout << "���⴮�ڳ�ʼ��ʧ��!" << endl;
	}
}

void LaserCtrlor::laser_on()
{
	unsigned char temp[1] = { 0x01 };
	this->comport.WriteData(temp, 1);
}

void LaserCtrlor::laser_off()
{
	unsigned char temp[1] = { 0x00 };
	this->comport.WriteData(temp, 1);
}
