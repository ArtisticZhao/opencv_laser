#pragma once
#include <Eigen/Dense>
#include<math.h>
#include "zhenjing_control.h"
using namespace Eigen;
class PlanB
{
public:
	void find(Vector3d a1, Vector3d a2, Vector3d b1, Vector3d b2);
	void move(Vector3d j, int* angles);
	void add_base_point(Vector3d base);
	void calc();
	void set_zj_ctrl(ZhenjingControlor* zjp);
private:
	Matrix4d t;
	short base_point_count = 0;
	Vector3d base_points[2];
	Vector3d origin_points[2] = {Vector3d(19.6221, 51.7276, -6.15337), Vector3d(23.9508, 58.2663, -5.28088)};
	bool is_calc_mode = false;  // 当第三次进入 add_base_point 函数之后, 进入计算模式, 将自动进入计算模式
	ZhenjingControlor* p_zhenjingctrl;
};

