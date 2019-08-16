#include "triangle_cxf.h"
#include <math.h>
#include <iostream>

const double pi(3.1415926535);

double triangle(double x, double y, double ar, double c, double f)
{
	double s1, s2, s, q, L;
	q = ar * pi / 180;
	s1 = f / tan(q);
	s2 = sqrt(x * x + y * y);
	s = s1 + s2 * 44e-6;
	L = c * f / s;
	return L;

}

double measuref(double x, double y,double c, double L, double ar) {
	double c1, c2, s2, s,f, q;
	q = ar * pi / 180;
	c1 = L / tan(q);
	c2 = c - c1;
	s = sqrt(x * x + y * y);
	s2 = s * 44.0e-6;
	f = L * s2 / c2;
	return f;
}



//double *zmeasure(double x,double y,double bx,double by,double L,double bi,double z[3])

//double *zmeasure(double x,double y,double bx,double by,double L,double bi,double a[], int n)
void zmeasure(double x, double y, double bx, double by, double L, double bi, double *a, int n, double angle)
{
	//printf("in: (%f ,%f), bx: %f, by: %f, %f,%f\n", x, y, bx, by, L, bi);
	double qx, zx, qy, zy;
	double z[3];
	qx = bx * pi / 180;
	qy = by * pi / 180;
	if (x == 0)
	{
		zx = L * tan(qx);
	}
	else if (bx == 90)
	{
		zx = bi * L / x;
	}
	else
	{
		zx = L / (x / bi + 1 / tan(qx));
	}
	zy = zx * y / bi;
	z[0] = zx * x / bi;
	z[1] = zx;
	z[2] = -zy;
	//std::cout << z[0] << " " << z[1] << " " << z[2] << std::endl;
	Vector3d before(z[0], z[1], z[2]);
	Vector3d after = find(before, -angle);
	for (int i = 0; i < n; i++)
		a[i] = after(i);

	//std::cout << a[0] << " " << a[1] << " " << a[2] << std::endl;
}

Vector3d find(Vector3d before, double angle)
{
	double pi = 3.14159265358979323846;           // pi;
	Vector4d before_qi;                           //Æë´Î×ø±ê
	before_qi(0) = before(0) - 14.2132;
	before_qi(1) = before(1) - 51.1912;
	before_qi(2) = before(2);
	before_qi(3) = 1;
	Vector4d later_qi;
	Matrix4d spin(4, 4);                          //Ðý×ª¾ØÕó
	spin(0, 0) = cos(angle);
	spin(1, 0) = -sin(angle);
	spin(2, 0) = 0;
	spin(3, 0) = 0;
	spin(0, 1) = sin(angle);
	spin(1, 1) = cos(angle);
	spin(2, 1) = 0;
	spin(3, 1) = 0;
	spin(0, 2) = 0;
	spin(1, 2) = 0;
	spin(2, 2) = 1;
	spin(3, 2) = 0;
	spin(0, 3) = 0;
	spin(1, 3) = 0;
	spin(2, 3) = 0;
	spin(3, 3) = 1;
	later_qi = spin * before_qi;
	Vector3d later;
	later(0) = later_qi(0) + 14.2123;
	later(1) = later_qi(1) + 51.1912;
	later(2) = later_qi(2);

	return later;
}
