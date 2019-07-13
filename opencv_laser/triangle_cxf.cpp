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
void zmeasure(double x, double y, double bx, double by, double L, double bi, double *a, int n)
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
	for (int i = 0; i < n; i++)
		a[i] = z[i];
}