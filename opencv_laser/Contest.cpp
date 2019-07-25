#include "Contest.h"
#include <time.h>
#include "obj_generator.h"
#include "LaserCtrlor.h"
#include <opencv.hpp>
extern LaserCtrlor lz_ctrl;
extern OBJ_Model objmodel;
extern double d3[3];
extern double uv[2];
extern double point_x, point_y;

extern void process_key(int key);
extern bool measure_lazer();

Contest::Contest()
{
	this->read_uvs();
	this->read_volts();
}

void Contest::set_zhenjing_ctrl(ZhenjingControlor* zj_c)
{
	this->zjp = zj_c;
}
void Contest::read_volts()
{
	ifstream volts_file;
	volts_file.open("data/volts.txt");
	char cmd[100];
	char* p;
	Vector2i volt;
	vector<Vector2i>().swap(this->volts);  // Çå¿Õ
	while (volts_file.getline(cmd, 100)) {
		p = cmd;
		volt(0) = stoi(p);
		p = strchr(p, ' '); p++;
		volt(1) = stoi(p);
		this->volts.push_back(volt);
	}
}
void Contest::read_uvs()
{
	ifstream uvs_file;
	uvs_file.open("data/uvs.txt");
	char cmd[100];
	char* p;
	Vector2i xy;
	while (uvs_file.getline(cmd, 100)) {
		p = cmd;
		xy(0) = stoi(p);
		p = strchr(p, ' '); p++;
		xy(1) = stoi(p);
		this->xys.push_back(xy);
	}
}

void Contest::goal_target(int x, int y, int volt_x, int volt_y)
{
	srand((unsigned)time(NULL));
	int times =  (rand() % (0 - 2 + 0)) + 2;
	for (int i = 0; i < times; i++) {
		bool has_point = measure_lazer();
		zjp->set_target(x, y);
		zjp->goal_target(point_x, point_y, true, true, has_point);
	}
	measure_lazer();
	zjp->goto_volt(volt_x, volt_y);
	measure_lazer();
}

void Contest::test()
{
	this->goal_target(xys.at(0)(0), xys.at(0)(1), volts.at(0)(0), volts.at(0)(1));
}

void Contest::show_all()
{
	for (int i = 0; i < this->volts.size(); i++) {
		this->goal_target(xys.at(i)(0), xys.at(i)(1), volts.at(i)(0), volts.at(i)(1));
		cout << "---------------" << endl;
		bool res = measure_lazer();
		int key = 0;
		while(key != 'v')
		{
			measure_lazer();
			key = cv::waitKey(1);
			process_key(key);
		}
	}
}

void Contest::adjust()
{
	lz_ctrl.laser_on();
	int key = 0;
	while (key != 'q') {
		zjp->goto_volt(this->volts.at(0)(0), this->volts.at(0)(1));
		key = cv::waitKey(100);
		zjp->goto_volt(this->volts.at(1)(0), this->volts.at(1)(1));
		key = cv::waitKey(100);
		zjp->goto_volt(this->volts.at(2)(0), this->volts.at(2)(1));
		key = cv::waitKey(100);
		zjp->goto_volt(this->volts.at(3)(0), this->volts.at(3)(1));
		key = cv::waitKey(100);
		zjp->goto_volt(this->volts.at(4)(0), this->volts.at(4)(1));
		key = cv::waitKey(100);
		zjp->goto_volt(this->volts.at(5)(0), this->volts.at(5)(1));
		key = cv::waitKey(100);
	}
}
