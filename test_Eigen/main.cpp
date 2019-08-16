#include <Eigen/Dense>
#include <iostream>
using namespace std;
using namespace Eigen;

void main() {
	Vector3d turn_axis(14.2123, 51.1912, -8.05424);
	Vector4d turn_axis1(14.2123, 51.1912, -8.05424, 1);
	Translation3d translation(-turn_axis);
	cout << turn_axis << endl;
	turn_axis = translation * turn_axis;
	cout << turn_axis << endl;
}