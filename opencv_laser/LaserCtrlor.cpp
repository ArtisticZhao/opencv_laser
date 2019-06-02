#include "LaserCtrlor.h"

LaserCtrlor::LaserCtrlor(int com)
{
	this->comport.InitPort(com, CBR_115200);
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
