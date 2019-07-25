#pragma once
#include <iostream>
#include <vector>
#include <fstream>
using namespace std;
#include <Eigen/Dense>
using namespace Eigen;

#include "zhenjing_control.h"


class Contest
{
private:
	vector<Vector2i> volts;
	vector<Vector2i> xys;
	ZhenjingControlor* zjp;
public:
	Contest();
	void set_zhenjing_ctrl(ZhenjingControlor* zj_c);
	void read_volts();
	void read_uvs();
	void goal_target(int x, int y, int volt_x, int volt_y);
	void test();
	void show_all();
	void adjust();
};

