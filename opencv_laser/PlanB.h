#pragma once
#include <Eigen/Dense>
#include<math.h>
using namespace Eigen;
class PlanB
{
public:
	void find(Vector3d a1, Vector3d a2, Vector3d b1, Vector3d b2);
	void move(Vector3d j, int* angles);
private:
	Matrix4d t;
};

