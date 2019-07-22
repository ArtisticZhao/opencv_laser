#include "PlanB.h"
void PlanB::find(Vector3d a1, Vector3d a2, Vector3d b1, Vector3d b2)
{
	double c, ar, f, d;
	Vector4d a;                       //移动前向量
	a(0) = b1(0) - a1(0);
	a(1) = b1(1) - a1(1);
	a(2) = b1(2) - a1(2);
	a(3) = 1;
	Vector4d b;                       //移动后向量
	b(0) = b2(0) - a2(0);
	b(1) = b2(1) - a2(1);
	b(2) = b2(2) - a2(2);
	b(3) = 1;
	Vector4d a0;                     //a1移动前齐次坐标
	a0(0) = a1(0);
	a0(1) = a1(1);
	a0(2) = a1(2);
	a0(3) = 1;                      //a1移动后齐次坐标
	Vector4d b0;
	b0(0) = a2(0);
	b0(1) = a2(1);
	b0(2) = a2(2);
	b0(3) = 1;

	Vector4d s;                   //a1点绕z轴旋转后的齐次坐标
	Vector4d p;                   //旋转后需移动的向量
	c = a(0) * b(0) + a(1) * b(1) + a(2) * b(2);
	d = sqrt(a(0) * a(0) + a(1) * a(1) + a(2) * a(2));
	f = sqrt(b(0) * b(0) + b(1) * b(1) + b(2) * b(2));
	ar = acos(c / (d * f));
	Matrix4d m(4, 4);
	m(0, 0) = cos(ar);
	m(1, 0) = sin(ar);
	m(2, 0) = 0;
	m(3, 0) = 0;
	m(0, 1) = -sin(ar);
	m(1, 1) = cos(ar);
	m(2, 1) = 0;
	m(3, 1) = 0;
	m(0, 2) = 0;
	m(1, 2) = 0;
	m(2, 2) = 1;
	m(3, 2) = 0;
	m(0, 3) = 0;
	m(1, 3) = 0;
	m(2, 3) = 0;
	m(3, 3) = 1;
	s = m * a0;
	p = b0 - s;
	Matrix4d pin(4, 4);
	pin(0, 0) = 1;
	pin(1, 0) = 0;
	pin(2, 0) = 0;
	pin(3, 0) = 0;
	pin(0, 1) = 0;
	pin(1, 1) = 1;
	pin(2, 1) = 0;
	pin(3, 1) = 0;
	pin(0, 2) = 0;
	pin(1, 2) = 0;
	pin(2, 2) = 1;
	pin(3, 2) = 0;
	pin(0, 3) = p(0);
	pin(1, 3) = p(1);
	pin(2, 3) = p(2);
	pin(3, 3) = 1;
	t = pin * m;

}

void PlanB::move(Vector3d j, int* angles)
{
	double pi = 3.14159265358979323846;// pi;
	double arx, ary, thx, thy, vx, vy, bix, biy;
	Vector4d b3;      //测试点的齐次坐标
	Vector4d pb;                  //测试点移动后齐次坐标
	b3(0) = j(0);
	b3(1) = j(1);
	b3(2) = j(2);
	b3(3) = 1;
	pb = t * b3;
	arx = atan(pb(0) / pb(1));
	ary = atan(pb(2) / pb(1));
	thx = arx / pi * 180;
	thy = ary / pi * 180;
	vx = (thx / 2 - 90) * 0.8;
	vy = (thy / 2 - 90) * 0.8;
	bix = vx / 10 * 32767;
	biy = vy / 10 * 32767;
	angles[0] = (int)bix;
	angles[1] = (int)biy;
}
